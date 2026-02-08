# Compiling

This guide will walk you through compiling the **Granite Engine Source Code** on your machine:

---

### Preparing

First of all, we need to do some things to get us done to start compiling:

1. Get the source code from the official github repository ([follow this guide](./cloning.md)).
2. Install all dependencies needed ([listed here](./dependencies.md)).
3. Open your Terminal in the source code root folder.

---

### Compiling

Now its time to actually compile the source code, to do so, you can follow this simple steps:

1. Run ```make clean``` to ensure a clean build.

   - Run ```make``` for a debug/dev build.

   - Run ```make release``` for a release build.

   - Run ```make ci``` for a CI test.

2. Done, now the **.a** file should be at **/lib** folder and **.o and .d** files at **/build** folder.

---
