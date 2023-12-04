import asyncio
from websockets.server import serve
from time import monotonic

async def wshandler(websocket):
    curr_time = monotonic()
    if curr_time % 3 == 0:
        await websocket.send("Halo")

    async for message in websocket:
        await websocket.send(message)
        print("Received message: " + message)

async def main():
    async with serve(wshandler, "localhost", 8765):
        await asyncio.Future()

asyncio.run(main())