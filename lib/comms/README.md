# So, You Want to Comms: A Guide

PSP-AC vehicles use the MAVLink communication protocol to process
commands, send telemetry, and more. This protocol allows us to robustly
communicate data in many formats, ensure that messaging protocol versions
are up to date, and recover from data corruption.

## What is MAVLink?

[MAVLink](https://mavlink.io/en/) stands for Micro Air Vehicle Link, and is commonly used
in the open-source drone ecosystem. However, it is also easily adaptable to
space vehicles; some satellite companies use MAVLink-controlled Ardupilot-based
systems in their satellites. In our case, we're using it to control rockets.

Since MAVLink lets us build our own custom message set, we've cherry-picked
the useful messages from the default one, deleted all the drone-related stuff,
added some other messages for our needs, and created the new **PSCom** message set. More on it below.

## But why not roll your own protocol?

Just don't, it's not worth the hassle of building CRCs and making sure
everyone's client library is up to date.

## Great! How do I start?

Luckily for us, MAVLink handles pretty much all the lower-level parts of communication.
What we need to do is define a *message set*, or all of the different types of messages
we're going to use. We will also define some *message exchanges* (called *microservices* in MAVLink)
to do more complex behavior. Some examples of message exchanges include:
- [The Command Protocol](https://mavlink.io/en/services/command.html) (cherry-picked from common)
- [The Parameter Protocol](https://mavlink.io/en/services/parameter.html) (UNIMPLEMENTED, cherry-picked from common)
- [The Mission Protocol](https://mavlink.io/en/services/mission.html) (*technically* cherry-picked from common, but we changed what a lot of the messages do, so we consider this a custom microservice protocol)

### Defining a Message Set

In MAVLink, we define a message set in XML. This allows us to generate code for using this message set
in many different languages.

#### Define a Message

The core of the message set is the message definition:
```xml
<message id="51820" name="TRAJ_ACK">
  <description>Send a response regarding whether the trajectory load was successful.</description>
  <field type="uint32_t" name="points_loaded">Number of points successfully loaded (-1 if error).</field>
  <field type="uint8_t" name="result" enum="MAV_RESULT">Result of load.</field>
</message>
```
This goes under the `<messages>` section. Let's look at the components:
- Each message has an `id` numerical number to identify what type of message it is.
- The message also has an English name associated with it. MAVLink will generate an enum with the name mapping to the number ID, so you won't need to use the number directly usually.
- The message is given a description, used only in generating documentation.
- The message is given fields, with types, a name and description associated.

We can define as many messages as we want (technically up to a few million).

#### Define an Enum

You may have noticed the `result` field has an `enum=` associated. We can define enums
with expected values that we can use in code:
```xml
<enum name="MAV_RESULT">
  <description>Result from a MAVLink command (MAV_CMD)</description>
  <entry value="0" name="MAV_RESULT_ACCEPTED">
    <description>Command is valid (is supported and has valid parameters), and was executed.</description>
  </entry>
  <entry value="1" name="MAV_RESULT_TEMPORARILY_REJECTED">
    <description>Command is valid, but cannot be executed at this time. This is used to indicate a problem that should be fixed just by waiting (e.g. a state machine is busy, can't arm because have not got GPS lock, etc.). Retrying later should work.</description>
  </entry>
  <entry value="2" name="MAV_RESULT_DENIED">
    <description>Command is invalid (is supported but has invalid parameters). Retrying same command and parameters will not work.</description>
  </entry>
</enum>
```

This is pretty self-explanatory.

**NOTE:** If you're adding a command, all commands use the same
`COMMAND_LONG` message type. To define a new command, you instead add
a new entry to the `MAV_CMD` enum.

### Using it in the Code

See the README in `../lib` for how to generate the messages. 

**CAVEAT**:Vehicles use the `fastmavlink` library to generate the code,
which is actually different from the `mavlink` library used to generate
code for the Java GCS. `fastmavlink` offers some optimizations over
the original library, but as you'll see, the API works pretty much the same.

#### Adding Handlers

The magic happens in the *processMessage* method:

```cpp
void CommsManager::processMessage(fmav_message_t *msg) {
    switch (msg->msgid) {
        case FASTMAVLINK_MSG_ID_COMMAND_LONG:
            fmav_command_long_t command;
            fmav_msg_command_long_decode(&command, msg);
            this->processCommand(msg->sysid, msg->compid, &command);
            break;
        case FASTMAVLINK_MSG_ID_PARAM_REQUEST_LIST:
            break; // Handling currently not implemented
        case FASTMAVLINK_MSG_ID_PARAM_REQUEST_READ:
            break; // Handling currently not implemented
        case FASTMAVLINK_MSG_ID_PARAM_SET:
            break; // Handling currently not implemented
        case FASTMAVLINK_MSG_ID_TRAJ_LOAD_SD_CARD:
            break; // Handling currently not implemented
        default:
            break; // Handling currently not implemented
    }
}
```

This is basically a giant switch case that tests what type of
message to process. You should add your message here.

#### Adding Command Handlers

There is also a corresponding command handler:

```cpp
void CommsManager::processCommand(uint8_t sysid, uint8_t compid, fmav_command_long_t *cmd) {
    // TODO: More generic method of registering handlers
    switch (cmd->command) {
        case MAV_CMD_DO_PAUSE_CONTINUE: {
            break;
        }
        default: {
            break;
        }
    }
}
```

Same deal - if you're adding a command instead of a message, add
a handler here.

#### Command Handlers - COMMAND_ACK

As part of the Command Protocol, all commands must return a COMMAND_ACK
message about whether the command succeeded. [See here](https://mavlink.io/en/messages/common.html#COMMAND_ACK).

#### Debug Printing - Status Text

At some point, you'll need to print debug messages, but you can't use
`Serial.println()` because it'll mess with the MAVLink data stream. But have no fear!
We can use `commsManager.sendStatusText(MAV_SEVERITY_INFO, "Here's text")` to send a debug message through MAVLink.

If you need to add in additional text, you can use Arduino's `String()` and use `.c_str()` to convert a string to the C string format.
Or, you can use sprintf.
