package main

import (
	"fmt"
	"net"
	"os"
	"os/signal"
	"sync/atomic"
	"syscall"
)

type Daemon struct {
	addr        string
	activeConns int64
}

func NewDaemon(addr string) *Daemon {
	return &Daemon{addr: addr}
}

func (d *Daemon) Start() error {
	ln, err := net.Listen("tcp", d.addr)
	if err != nil {
		return err
	}
	defer ln.Close()

	fmt.Printf("[nexusd] Multiplexer router listening on %s\n", d.addr)
	for {
		conn, err := ln.Accept()
		if err != nil {
			return err
		}
		atomic.AddInt64(&d.activeConns, 1)
		go d.handleConn(conn)
	}
}

func (d *Daemon) handleConn(conn net.Conn) {
	defer func() {
		conn.Close()
		atomic.AddInt64(&d.activeConns, -1)
	}()
	buf := make([]byte, 4096)
	for {
		n, err := conn.Read(buf)
		if err != nil {
			return
		}
		// Echo back
		if _, err := conn.Write(buf[:n]); err != nil {
			return
		}
	}
}

func main() {
	d := NewDaemon(":9190")
	sig := make(chan os.Signal, 1)
	signal.Notify(sig, syscall.SIGINT, syscall.SIGTERM)

	go func() {
		<-sig
		fmt.Println("\n[nexusd] Shutting down daemon gracefully...")
		os.Exit(0)
	}()

	if err := d.Start(); err != nil {
		fmt.Fprintf(os.Stderr, "Daemon runtime error: %v\n", err)
	}
}
