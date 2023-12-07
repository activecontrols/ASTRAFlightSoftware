# Lib

All support libraries for the ASTRA system are stored under this directory. This includes
the LQR controller and communications code.

The MAVLink message definitions are also contained in this directory. To auto-generate HTML
documentation for these, go to `../docs/`.

For a comprehensive guide on using MAVLink and modifying the comms code, see the README under
`comms/`.

## Writing New Messages

Communications message definitions are located under message_definitions/1.0/pscom.xml. Enums and
messages are defined in XML. You should be to get a good idea of how it works from the structure alone.

Before writing a new message, check if [a standard MAVLink message already suits your needs](https://mavlink.io/en/messages/common.html).
The [common definitions](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/common.xml) can be
referenced both for an idea of how to write new messages and for copying in existing messages.

We do not base directly off common.xml because it contains a large message set, many of which are not related to
ASTRA operation. 

**However, if you feel this would be a good path, copy common.xml into the `message_definitions/1.0` path and change the <include /> in pscom.xml to use it.**

Once you have edited the message definitions, you must regenerate the C library.

### (Re)Generating Messages

To generate a new C library based on the XML message definition:
1. `pip3 install -r ../requirements.txt`
2. `python3 fastmavlink/fmav_gui.py`
3. In the resulting GUI, for Message Definitions navigate to the aforementioned pscom.xml.
4. For output, name it to `message_lib` (there should already be one, you can name it to the same to replace). Make sure it is in this directory and **not** inside `fastmavlink/`.
5. Click "Generate".

### Using the generated message library

Using is pretty simple. Here's an example:
```c
#include "message_lib/pscom/pscom.h"

```

## Original PlatformIO Content

This directory is intended for project specific (private) libraries.
PlatformIO will compile them to static libraries and link into executable file.

The source code of each library should be placed in a an own separate directory
("lib/your_library_name/[here are source files]").

For example, see a structure of the following two libraries `Foo` and `Bar`:

|--lib
|  |
|  |--Bar
|  |  |--docs
|  |  |--examples
|  |  |--src
|  |     |- Bar.c
|  |     |- Bar.h
|  |  |- library.json (optional, custom build options, etc) https://docs.platformio.org/page/librarymanager/config.html
|  |
|  |--Foo
|  |  |- Foo.c
|  |  |- Foo.h
|  |
|  |- README --> THIS FILE
|
|- platformio.ini
|--src
   |- main.c

and a contents of `src/main.c`:
```
#include <Foo.h>
#include <Bar.h>

int main (void)
{
  ...
}

```

PlatformIO Library Dependency Finder will find automatically dependent
libraries scanning project source files.

More information about PlatformIO Library Dependency Finder
- https://docs.platformio.org/page/librarymanager/ldf.html
