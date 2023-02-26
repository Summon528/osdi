import sys
import time

assert(len(sys.argv) == 3)

with open(sys.argv[1], "wb+", buffering = 0) as tty:
    with open(sys.argv[2], "rb") as f:
        data = bytearray(f.read())
        size = len(data)
        print(f"Sending, size = {size}")
        tty.write(size.to_bytes(4,"big"))
        for idx, i in enumerate(data):
            tty.write(i.to_bytes(1, "big"))
            time.sleep(0.001)
            print(f"{round(((idx+1) / len(data)) * 100)}%", end='\r')

print("Done")
