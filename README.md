# Cumulocity Linux Agent #

Cumulocity Linux agent is a generic agent for connecting Linux-powered devices to Cumulocity's IoT platform. It runs on all major Linux distributions (Ubuntu, Debian, Raspbian, CentOS, etc.).

### Current Features ###

* Periodically report memory usage to Cumulocity.
* Periodically report system load to Cumulocity.
* Send log files (dmesg, syslog, journald, agent log) to Cumulocity on demand.
* Modbus-TCP and Modbus-RTU
* CANopen (using SocketCAN interface)

### How to build the agent? ###

* Prerequisites:
    - Linux (>= 2.6.32)

    - liblua (>= 5.1)

    - libcurl (>= 7.26.0)

    - Systemd (optional, for automatically starting the agent on boot)
    
    - LuaSocket (optional, only for the use of CANopen)

* Build the [Cumulocity C++ library](https://bitbucket.org/m2m/cumulocity-sdk-c) with the default *Makefile* and *common.mk* as *init.mk*.
* Download the agent source code:

```bash
git clone git@bitbucket.org:m2m/cumulocity-agents-linux.git

```

* Export the Cumulocity C++ library path (add the following code to your ~/.bashrc for permanence):

```bash
export C8Y_LIB_PATH=/library/root/path
```
* Copy the compiled library files to the *lib/* directory under the agent root directory.

```bash
cd cumulocity-agents-linux
cp -rP $C8Y_LIB_PATH/lib $C8Y_LIB_PATH/bin .
```

* Build the agent:
```bash
make # in *debug* mode:
```

Or

```bash
make release # in *release* mode:
```

* Install the agent:
```
sudo make install
```

* Run the agent:

```bash
sudo cumulocity-agent
```

Or for Linux distributions with systemd:

```bash
sudo systemctl enable cumulocity-agent
sudo systemctl start cumulocity-agent
```

#### How to enable Modbus support in the agent ####
Modbus support is by default disabled. To enable modbus support, you need to edit the Makefile to set`PLUGIN_MODBUS:=1`.
The modbus feature requires libmodbus library, make sure you have libdmobus installed before build with modbus support.

#### How to enable CANopen support in the agent ####
CANopen is composed of two parts. A Lua plugin, which is included into the agent build by default. However, to actually
get CANopen support, you would also need to build the Cumulocity CANopen service, which is a C program based on the
CANopen library and SocketCAN connector from Port Automation.

The CANopen library and SocketCAN connector is commercially licensed by Port, and are not included in the repo. You
would need to get them from Port if you want to build the Cumulocity CANopen service.

Assume you have the CANopen library and SocketCAN connector available, you need to create a directory `ext/port` in the
repo and exatrct the zip files there. After the extracion, your `ext/port` folder should have the following strcture:

```bash
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

To build and run the Cumulocity CANopen service:

```bash
$ cd canopen
$ make
$ cd ..
$ ./bin/c8y_canopend
```

The Cumulocity CANopen service needs NO configuration. It's communicating with the Linux Agent via UDP port 9677. It
gets all configuration, including SocketCAN interface, baudrate etc. automatically from the Linux Agent, so you just
need to adjust all the CANopen settings in the Linux Agent.

There is also a CANopen simulator included in the repo for testing purpose. To build and run it:

```bash
$ cd tools/canopen_simulator
$ make
$ ./c8y_canopen_simulator 5 0
```

Note 5 is the CANopen Node ID that you want the simulator to run with, and 0 is the CAN interface number, i.e., can0. 
In this example, the simulator is automatically connected to SocketCAN interface `can0`, 
make sure that you have a proper can0 CAN interface, or use the default CANopen settings
in the Linux Agent to have the agent creates a vcan can0 interface for you.

### FAQ ###

#### How can I uninstall the agent? ####

Run:

```bash
sudo make uninstall
```

#### How can I build RPM of the agent? ####

After building of the agent run:

```bash
sudo make rpm -v 0.1 # replace 0.1 with required version number
```
The RPM is saved in folder build/rpm.

#### How can I build and package the agent as a Ubuntu snap? ####

  The requirements are the same as above, but the build requires special treatment because Ubuntu Snap Core uses its
  own file system structure, instead do:

```bash
make release  PREFIX=/snap/cumulocity-agent/current/usr DATAPATH=/var/snap/cumulocity-agent/common
make snap PREFIX=/snap/cumulocity-agent/current/usr DATAPATH=/var/snap/cumulocity-agent/common
```

This will create the snap package. Then the agent needs to be installed in developer mode, since snap sandboxing is
currently too restrictive. To install it run:

```bash
sudo snap install <agent.snap> --devmode
```

The agent starts automatically after installation, also at every time the machine boots.

NOTE: packaging requires snapcraft >= 2.10 because lower versions do not support the confinement property, which is
required for packaging the agent as a snap.

