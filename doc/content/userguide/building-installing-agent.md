---
title: "Building and installing the Cumulocity Linux Agent"
draft: false
weight: 103
---

Before starting anything, make sure that you have compiled Cumulocity C++ SDK.
If you want the **Cloud Remote Access** feature, please also refer to [Building the Cumulocity Cloud Remote Access Service](#building-the-cumulocity-cloud-remote-access-service).
If you want the **Modbus** support, please make sure that you have installed `libmodbus` and `LuaSocket` packages. The details for enabling Modbus support is described in [Building the agent with a Modbus support](#building-the-agent-with-modbus-support).
Also, for **CANopen** support, check if you have `CANopen library` and `SocketCAN connector` commercially licensed by [port industrial automation GmbH](https://www.port.de/en/products/canopen/software.html) as well as `LuaSocket` package. Please refer to  [Building the Cumulocity CANopen Service](#building-the-cumulocity-cloud-remote-access-service) to get more details.

### Building the basic agent
This section guides how to build the Cumulocity Linux Agent without Modbus support.

1. Copy the Cumulocity Linux Agent repository to a directory of your choice.
    ```shell
    cd ~/<my_working_directory>
    git clone  https://bitbucket.org/m2m/cumulocity-agents-linux.git
    ```

2. Export the SDK binaries and libraries path (i.e.: _/home/me/repos/cumulocity-sdk-c_). Preferably add the following code to your _~/.bashrc_ for permanence).
    ```shell
    export C8Y_LIB_PATH=/path/to/cumulocity-sdk-c
    ```

3. Enter the cumulocity-agents-linux directory and copy the compiled binaries and libraries of the SDK by copying to the _bin_ and _lib_ directories from the SDK.
    ```shell
    cd cumulocity-agents-linux
    cp -rP $C8Y_LIB_PATH/lib $C8Y_LIB_PATH/bin .
    ```

4. Customize your _Makefile_ and correct the libraries names. If you installed the `liblua5.3-dev` then modify two **lua** to **lua5.3** in `CPPFLAGS` and `LDLIBS` lines.

   From
    ```shell
    CPPFLAGS+=-I$(C8Y_LIB_PATH)/include $(shell pkg-config --cflags lua)\
                      -DPKG_DIR='"$(PKG_DIR)"'
    LDLIBS:=-lsera $(shell pkg-config --libs lua) -pthread
    ```
    to
    ```shell
    CPPFLAGS+=-I$(C8Y_LIB_PATH)/include $(shell pkg-config --cflags lua5.3)\
                      -DPKG_DIR='"$(PKG_DIR)"'
    LDLIBS:=-lsera $(shell pkg-config --libs lua5.3) -pthread
    ```

5. To build the agent in debug mode, run:
    ```shell
    make
    ```
    For production, to build in release mode, run:
    ```shell
    make release
    ```

### Building the Cumulocity Cloud Remote Access Service
This agent supports the Cloud Remote Access feature. If your device supports VNC, Telnet, or SSH remote access, you can remotely manage it via Cumulocity IoT. For details on the remote access functionality, refer to [Cloud Remote Access](https://cumulocity.com/guides/users-guide/optional-services#cloud-remote-access).

To support the feature, you need to build the **Cumulocity Cloud Remote Access Service** aside from building the agent. To build it, run:
```shell
make vnc
```
Now you have an execution file `vncproxy` in _cumulocity-agents-linux/bin_.

The Cumulocity Cloud Remote Access service needs no further configuration. It communicates with the Linux Agent via a local socket.

### Building the agent with Modbus support
Modbus support is disabled by default. In between step 4 and step 5 of [Building the basic agent](#basicagent), you need to do one additional step. The Modbus feature requires `libmodbus` library, so that make sure you have `libdmobus` installed before building with Modbus support.

After step 4 of [Building the basic agent](#basicagent), edit your _Makefile_ file and set `PLUGIN_MODBUS` to `1`. By default, this variable is set `0` (disabled).

```shell
PLUGIN_MODBUS:=1
```

After you finished this step, continue step 5 of [Building the basic agent](#building-the-basic-agent) to build the agent.

### Building the Cumulocity CANopen Service
CANopen support is also disabled by default. After you finish all steps of [Building the basic agent](#building-the-basic-agent), you need to do a couple of additional steps.

CANopen support is composed of two parts. One is a Lua plugin, which is included the agent repository by default. However, to get actual CANopen support, you would also need to build the **Cumulocity CANopen Service**, which is a C program based on the **CANopen library** and **SocketCAN connector** from [port industrial automation GmbH](https://www.port.de/en/products/canopen/software.html).

The CANopen library and SocketCAN connector is commercially licensed by [port industrial automation GmbH](https://www.port.de/en/products/canopen/software.html), and are not included in this repository. You would need to get them from them if you want to build the Cumulocity CANopen Service.

Assume you have the CANopen library and SocketCAN connector available, you need to create a directory _ext/port_ in the repository and extract the zip files there. After the extraction, your _ext/port_ directory should have the following structure:

```shell
$ ls -hl ext/port/
drwxr-xr-x 1 tiens tiens   44 Nov 27 16:45 canopen
drwxr-xr-x 1 tiens tiens   80 Nov 27 16:45 drivers
$ ls -hl ext/port/canopen
-rw-r--r-- 1 tiens tiens 11K Nov 27 16:45 CHANGELOG
drwxr-xr-x 1 tiens tiens 664 Nov 27 16:45 include
drwxr-xr-x 1 tiens tiens 834 Nov 27 16:45 source
$ ls -hl ext/port/drivers
-rw-r--r-- 1 tiens tiens  21K Nov 27 16:45 CHANGELOG_DRV
drwxr-xr-x 1 tiens tiens   92 Nov 27 16:45 linux
-rw-r--r-- 1 tiens tiens 7.3K Nov 27 16:45 README
drwxr-xr-x 1 tiens tiens  206 Nov 27 16:45 shar_inc
drwxr-xr-x 1 tiens tiens  106 Nov 27 16:45 shar_src
```

#### Building the Cumulocity CANopen Service

To build the Cumulocity CANopen Service, move to the repository root directory and run:
```shell
cd canopen
make
```
Then `c8y_canopend` execution file is created in _cumulocity-agents-linux/bin_.

The Cumulocity CANopen service communicates with the Linux Agent via UDP port 9677. It gets all configuration, including SocketCAN interface, baud rate etc. automatically from the Linux Agent, so you just need to adjust all the CANopen settings in the Linux Agent configuration file (_cumulocity-agent.conf_ file) later in the section [Configuring the agent](./../configuringagent).

### Installing the agent
{{% notice info %}}
Before you install the agent, you need to configure the agent parameters in _cumulocity-agent.conf_ file. Please refer to [Configuring the agent](./../configuringagent) Section.
{{% /notice %}}

Regardless of whether your agent supports Modbus, CANopen or none of them, you can install and uninstall the agent by the same commands.
After you build the agent, enter your _cuumulocity-agents-linux_ directory and run:
```shell
sudo make install
```
The agent's binary files, the configuration file(_cumulocity-agent.conf_), the SmartREST template file(_srtemplate.txt_), the systemd service file, and C++ SDK shared library files are now deployed to your device.

### Uninstalling the agent
In your _cuumulocity-agents-linux_ directory and run:
```shell
sudo make uninstall
```
The agent binary files, the configuration file(_cumulocity-agent.conf_), the SmartREST template file(_srtemplate.txt_), the systemd service file, and C++ SDK shared library files are now removed from your device.
