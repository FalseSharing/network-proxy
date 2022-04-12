import asyncio
import struct

class NexusFrame:
    HEADER_FMT = "!IBBI"  # magic (4B), version (1B), cmd (1B), payload_len (4B)
    MAGIC = 0x4E455853     # "NEXS"

    def __init__(self, cmd: int, payload: bytes = b""):
        self.version = 1
        self.cmd = cmd
        self.payload = payload

    def serialize(self) -> bytes:
        header = struct.pack(self.HEADER_FMT, self.MAGIC, self.version, self.cmd, len(self.payload))
        return header + self.payload

    @classmethod
    def deserialize(cls, data: bytes):
        if len(data) < struct.calcsize(cls.HEADER_FMT):
            return None, data
        magic, ver, cmd, plen = struct.unpack(cls.HEADER_FMT, data[:10])
        if magic != cls.MAGIC:
            raise ValueError(f"Invalid protocol magic: {hex(magic)}")
        total_len = 10 + plen
        if len(data) < total_len:
            return None, data
        payload = data[10:total_len]
        frame = cls(cmd, payload)
        frame.version = ver
        return frame, data[total_len:]

class AsyncNexusConnection:
    def __init__(self, host: str = "127.0.0.1", port: int = 9190):
        self.host = host
        self.port = port
        self.reader = None
        self.writer = None

    async def connect(self):
        self.reader, self.writer = await asyncio.open_connection(self.host, self.port)

    async def send_frame(self, frame: NexusFrame):
        self.writer.write(frame.serialize())
        await self.writer.drain()

    async def close(self):
        if self.writer:
            self.writer.close()
            await self.writer.wait_closed()
