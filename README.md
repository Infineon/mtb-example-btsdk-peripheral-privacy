# Bluetooth&reg; - LE Peripheral Privacy

This code example demonstrates the privacy features available to users in Bluetooth&reg; 5.0 and above using ModusToolbox™ software.

Features demonstrated:
1. Privacy modes as defined in Bluetooth&reg; spec 5.0 and above.
2. Use of persistent storage for bond data management.
3. Management and handling of bond data of multiple peer devices.

[View this README on GitHub.](https://github.com/Infineon/mtb-example-btsdk-peripheral-privacy)

[Provide feedback on this code example.](https://cypress.co1.qualtrics.com/jfe/form/SV_1NTns53sK2yiljn?Q_EED=eyJVbmlxdWUgRG9jIElkIjoiQ0UyMjg3NTEiLCJTcGVjIE51bWJlciI6IjAwMi0yODc1MSIsIkRvYyBUaXRsZSI6IkJsdWV0b290aCZyZWc7IC0gTEUgUGVyaXBoZXJhbCBQcml2YWN5IiwicmlkIjoiYW1tbCIsIkRvYyB2ZXJzaW9uIjoiMi4xLjAiLCJEb2MgTGFuZ3VhZ2UiOiJFbmdsaXNoIiwiRG9jIERpdmlzaW9uIjoiTUNEIiwiRG9jIEJVIjoiSUNXIiwiRG9jIEZhbWlseSI6IkJUQUJMRSJ9)


## Requirements

- [ModusToolbox&trade; software](https://www.cypress.com/products/modustoolbox-software-environment) v2.2 or later (tested with v2.3).

    **Note:** This code example version requires ModusToolbox&trade; software version 2.2 or later and is not backward compatible with v2.1 or older versions. If you cannot move to ModusToolbox&trade; software v2.2, use the latest compatible version of this example: [latest-v1.X](https://github.com/Infineon/mtb-example-btsdk-peripheral-privacy/tree/latest-v1.X).
- Board support package (BSP) minimum required version: 3.1.0
- Programming language: C

- Associated parts:  [AIROC™ CYW20819 Bluetooth® & Bluetooth® LE system on chip](https://www.cypress.com/products/cyw20819), [AIROC™ CYW20719 Bluetooth® & Bluetooth® LE system on chip](https://www.cypress.com/products/cyw20719)
, [AIROC™ CYW20735 Bluetooth® & Bluetooth® LE system on chip](https://www.cypress.com/products/cyw20735), [AIROC™ CYW20820 Bluetooth® & Bluetooth® LE system on chip](https://www.cypress.com/datasheet/CYW20820)

## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm® embedded compiler v9.3.1 (`GCC_ARM`) - Default value of `TOOLCHAIN`

## Supported kits (make variable 'TARGET')
- [CYW920819EVB-02 evaluation kit](http://www.cypress.com/CYW920819EVB-02) (`CYW920819EVB-02`) - Default value of `TARGET`
- [CYW920820EVB-02 evaluation kit](http://www.cypress.com/CYW920820EVB-02) (`CYW920820EVB-02`)
- [CYW920719B2Q40EVB-01 evaluation kit](https://www.cypress.com/documentation/development-kitsboards/cyw920719b2q40evb-01-evaluation-kit) (`CYW920719B2Q40EVB-01`)
- [CYW920735Q60EVB-01 evaluation kit](http://www.cypress.com/CYW920735Q60EVB-01) (`CYW920735Q60EVB-01`)


## Hardware setup

This example uses the board's default configuration. See the kit user guide to ensure that the board is configured correctly.


## Software setup

Install a terminal emulator if you don't have one. Instructions in this document use [Tera term](https://ttssh2.osdn.jp/index.html.en). All other required software come bundled with the Eclipse IDE for ModusToolbox&trade; software.

## Using the code example

Create the project and open it using one of the following:

<details><summary><b>In Eclipse IDE for ModusToolbox&trade; software</b></summary>

1. Click the **New Application** link in the **Quick Panel** (or, use **File** > **New** > **ModusToolbox Application**). This launches the [Project Creator](https://www.cypress.com/ModusToolboxProjectCreator) tool.

2. Pick a kit supported by the code example from the list shown in the **Project Creator - Choose Board Support Package (BSP)** dialog.

   When you select a supported kit, the example is reconfigured automatically to work with the kit. To work with a different supported kit later, use the [Library Manager](https://www.cypress.com/ModusToolboxLibraryManager) to choose the BSP for the supported kit. You can use the Library Manager to select or update the BSP and firmware libraries used in this application. To access the Library Manager, click the link from the **Quick Panel**.

   You can also just start the application creation process again and select a different kit.

   If you want to use the application for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.
3. In the **Project Creator - Select Application** dialog, choose the **LE_Peripheral_Privacy** application by enabling the checkbox.

4. (Optional) Change the suggested **New Application Name**.

5. The **Application(s) Root Path** defaults to the Eclipse workspace which is usually the desired location for the application. If you want to store the application in a different location, you can change the *Application(s) Root Path* value. Applications that share libraries should be in the same root path.

6. Click **Create** to complete the application creation process.

For more details, see the [Eclipse IDE for ModusToolbox&trade; software user guide](https://www.cypress.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/ide_{version}/docs/mt_ide_user_guide.pdf*).

</details>

<details><summary><b>In command-line interface (CLI)</b></summary>

ModusToolbox&trade; software provides the Project Creator as both a GUI tool and the command line tool, "project-creator-cli". The CLI tool can be used to create applications from a CLI terminal or from within batch files or shell scripts. This tool is available in the *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/* directory.

Use a CLI terminal to invoke the "project-creator-cli" tool. On Windows, use the command line "modus-shell" program provided in the ModusToolbox&trade; software installation instead of a standard Windows command-line application. This shell provides access to all ModusToolbox&trade; software tools. You can access it by typing `modus-shell` in the search box in the Windows menu. In Linux and macOS, you can use any terminal application.

This tool has the following arguments:

Argument | Description | Required/optional
---------|-------------|-----------
`--board-id` | Defined in the `<id>` field of the [BSP](https://github.com/Infineon?q=bsp-manifest&type=&language=&sort=) manifest | Required
`--app-id`   | Defined in the `<id>` field of the [CE](https://github.com/Infineon?q=ce-manifest&type=&language=&sort=) manifest | Required
`--target-dir`| Specify the directory in which the application is to be created if you prefer not to use the default current working directory | Optional
`--user-app-name`| Specify the name of the application if you prefer to have a name other than the example's default name | Optional

<br>

The following example will clone the "[Hello World](https://github.com/Infineon/mtb-example-psoc6-hello-world)" application with the desired name "MyHelloWorld" configured for the *CY8CKIT-062-WIFI-BT* BSP into the specified working directory, *C:/mtb_projects*:

   ```
   project-creator-cli --board-id CY8CKIT-062-WIFI-BT --app-id mtb-example-psoc6-hello-world --user-app-name MyHelloWorld --target-dir "C:/mtb_projects"
   ```

**Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For details, see the "Project creator tools" section of the [ModusToolbox&trade; software user guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>

<details><summary><b>In third-party IDEs</b></summary>

Use one of the following options:

- **Use the standalone [Project Creator](https://www.cypress.com/ModusToolboxProjectCreator) tool:**

   1. Launch Project Creator from the Windows Start menu or from *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/project-creator.exe*.

   2. In the initial **Choose Board Support Package** screen, select the BSP, and click **Next**.

   3. In the **Select Application** screen, select the appropriate IDE from the **Target IDE** drop-down menu.

   4. Click **Create** and follow the instructions printed in the bottom pane to import or open the exported project in the respective IDE.

<br>

- **Use command-line interface (CLI):**

   1. Follow the instructions from the **In command-line interface (CLI)** section to create the application, and then import the libraries using the `make getlibs` command.

   2. Export the application to a supported IDE using the `make <ide>` command.

   3. Follow the instructions displayed in the terminal to create or import the application as an IDE project.

For a list of supported IDEs and more details, see the "Exporting to IDEs" section of the [ModusToolbox&trade; software user guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>


## Operation

1. Connect the board to your PC using the provided USB cable through the USB connector.

2. Open a terminal program and select the  COM port. Set the serial port parameters to 8N1 and 115200 baud.

3. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE for ModusToolbox&trade; software</b></summary>

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program**.
   </details>

   <details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. The default toolchain and target are specified in the application's Makefile but you can override those values manually:
      ```
      make program TARGET=<BSP> TOOLCHAIN=<toolchain>
      ```
      Example:
      ```
      make program TARGET=CYW920819EVB-02 TOOLCHAIN=GCC_ARM
      ```
   </details>

4. The application runs a custom button service with one custom characteristic that counts the number of button presses on the kit. It can be read or setup for notifications. Each time the button on the kit is pressed the count value is incremented. If any device is connected and has notifications enabled the updated value is sent to it. If no device is connected or notifications are disabled a message informing the same is displayed.

  **NOTE:** The button count is incremented on the button press irrespective of whether any device is connected or not.

5. Following instructions appear on the terminal on application start :
    * Press **'l'** to check for the number of bonded devices and next empty slot
        - This Option allows you to see how many devices are paired to the peripheral and which is the next available slot. This example supports upto 4 bonded devices after which the oldest devices data is overwritten.
    * Press **'r'** to erase all the bond data present in NVRAM
        - This option allows user to clear the memory of all the present bond data.
    * Press **'e'** to enter the bonding mode and add devices to bond list
        - This option puts the peripheral into bonding mode allowing it to connect and bond with new devices. After connection and bonding the incoming device can read and subscribe to the custom button count service.
    * Press **'p'** to change privacy mode of bonded device
        - This is used to change the privacy mode setting of the bonded devices i.e to move devices from device privacy mode to netwok privacy mode and vice versa. For more information about the privacy modes read the design and implementation section.
    * Press **'h'** any time in application to print the menu
        - At any point in the program to look at the options available user can request the start menu options.

        Use these available commands to interact with the application. Refer [Figure 2](#Figure-2-Process-Flowchart) for the application flow chart.


## Design and implementation

The objective of the application is to demonstrate how the user can make use of privacy features available in the Bluetooth&reg; devices. Bluetooth&reg;l devices implement privacy mainly by using **different types of addresses**: public, which means no privacy, or random. Random addresses can be Static (which don't change) or private (which change periodically, offering privacy protection). Private addresses can be further divided into non-resolvable or resolvable.

The use of resolvable private addresses allows only the devices that are paired to your device to "identify" the device as a known device; all other devices will perceive the device as a new device, making it difficult to track. If the device uses non-resolvable private address, it will be perceived as a new device every time it changes its address. The address is changed at regular intervals and is configurable.

Every privacy-enabled Bluetooth&reg;LE device has a unique address called the identity address and an identity resolving key (IRK). The identity address is the public address or static address of the Bluetooth&reg;LE device. The IRK is used by the BLE device to generate its RPA and is used by peer devices to resolve the RPA of the BLE device. Both the identity address and the IRK are exchanged during the pairing process. Privacy-enabled BLE devices maintain a list that consists of the peer device’s identity address, the local IRK used by the BLE device to generate its RPA, and the peer device’s IRK used to resolve the peer device’s RPA. This is called the resolving list. Only peer devices that have the 128-bit identity resolving key (IRK) of a BLE device can determine the device's address.

Max Resolving list size supported on different devices is:
- CYW20819    : 20 entries
- CYW20820    : 20 entries
- CYW20735B1  : 128 entries
- CYW20719B2  : 128 entries

   **Figure 1. Bluetooth&reg; LE address types**

   ![](images/address_types.jpg)

Apart from this, Bluetooth&reg; 5.0 introduced more options in the form of **privacy modes**.

There are two modes of privacy: **device privacy** mode and **network privacy** mode.

A device in *device privacy* mode is only concerned about the privacy of the device and will accept advertising/connection packets from peer devices that contain their identity address as well as ones that contain a private address, even if the peer device has distributed its identity resolution key (IRK) in the past.

In *network privacy* mode, a device will only accept advertising/connection packets from peer devices that contain private addresses. By default, network privacy mode is used when private addresses are resolved and generated by the controller.

[Table 1](#table-1-logical-representation-of-resolving-list-entries) shows the logical representation of the resolving list when a device that is trying to reconnect the controller checks the resolving list for the device. Depending on the result two things can happen as follows:

1. Device is found in the list.

Advertisement/Connection Address Type| Privacy Mode  |Request Accepted/Rejected
-------------------------------------|---------------|-------------------
Identity address | Network| Rejected
Identity address | Device | Accepted
Resolvable private address| Network| Accepted
 Resolvable private address| Device| Accepted 

2. Device not found in the list.

    In this case, the incoming device is treated as a new device and the request is forwarded to the host by the controller for further processing.

**Note:** A device using non-resolvable private address will be treated as a new device on every reconnection.


 **Table 1. Logical representation of resolving list entries**

Device| Local IRK| Peer IRK| Peer Identity Address|Identity Address Type| Privacy Mode
------|----------|---------|----------------------|---------------------|-------------
1     |Local IRK | Peer 1 IRK| Peer 1 Identity Address|Static/Public| Network/Device
2     |Local IRK | Peer 2 IRK| Peer 2 Identity Address|Static/Public| Network/Device
3     |Local IRK | Peer 3 IRK| Peer 3 Identity Address|Static/Public| Network/Device

The application runs a custom button service with one custom characteristic that counts the number of button presses on the kit. It can be read or setup for notifications.The GATT DB is setup so that the characteristic can be read without pairing/bonding but for enabling and disabling notifications pairing/bonding is required. Each time the button on the kit is pressed the count value is incremented. If any device is connected and has notifications enabled the updated value is sent to it. If no device is connected or notifications are disabled a message informing the same is displayed.

The device can store bond data of upto 4 peer devices after which the data of the oldest device is overwritten by that of the new incoming device. The incoming device is added in device privacy mode by default which can be changed by issuing **'p'** command via terminal.

The peripheral has 5 states:

1. **IDLE_NO_DATA** : The device in this state is either waiting for the user input or advertising. No bond Data is present in the NVRAM. Directed advertising option is disabled in this state.
2. **IDLE_DATA** : The device in this state is either waiting for the user input or advertising. Bond Data is present in the NVRAM. Directed advertising option is available.
3. **IDLE_PRIVACY_CHANGE**: The device in this state is not advertising. The device enters this mode when command to change the privacy mode of bonded devices is issued.
4. **CONNECTED**: In this state the peripheral is connected to a peer device.
5. **BONDED**: The peripheral moves into this state once it has has paired and bonded with the connected device and the peer bond information has been saved to NVRAM.

The LED 1 on the kit is used to represent the current advertising state of the device. [Table 2](#table-2-led-behaviour-for-advertising-states) shows LED behaviour for different advertising states.

 **Table 2. LED behaviour for advertising states**
 
Advertisement state | LED state
------------------- | ----------
Advertisement ON (Undirected) | Slow Blinking led (T = 1 sec)
Advertisement ON (Directed):  | Fast Blinking led (T = 20msec)
Advertisement OFF, Connected: | LED ON
Advertisement OFF, Timed out: | LED OFF

   **Figure 2. Process flowchart**

   ![](images/process_flow_chart_directed.jpg)

<br>

## Resources and settings

This section explains the ModusToolbox&trade; software resources and their configuration as used in this code example. Note that all the configuration explained in this section has already been done in the code example. Eclipse IDE for ModusToolbox&trade; software stores the configuration settings of the application in the *design.modus* file. This file is used by the graphical configurators, which generate the configuration firmware. This firmware is stored in the application’s *GeneratedSource* folder.

- **Device configurator:** Used to enable/configure the peripherals and the pins used in the application. See the
[Device configurator guide](https://www.cypress.com/ModusToolboxDeviceConfig).

* **Bluetooth&reg; configurator:** Used for generating/modifying the Bluetooth&reg; LE GATT database. See the
[Bluetooth&reg; configurator guide](https://www.cypress.com/ModusToolboxBLEConfig).

<br>

## Related resources

Resources  | Links
-----------| ----------------------------------
Application notes  | [AN225684](https://www.cypress.com/an225684): Getting started with CYW208xxDescribes CYW208xx device and how to build your first ModusToolbox&trade; software project |
Code examples  | [Using ModusToolbox&trade; software](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software) on GitHub <br> [Using Bluetooth&trade; Creator](https://www.cypress.com/documentation/code-examples/bluetooth-sdk-code-examples)
Device documentation | [CYW20819 device datasheet](https://www.cypress.com/datasheet/CYW20819) <br> [CYW20820 device datasheet](https://www.cypress.com/datasheet/CYW20820) <br> [CYW20719 device datasheet](https://www.cypress.com/documentation/datasheets/cyw20719-enhanced-low-power-bredrble-bluetooth-50-soc) <br> [CYW20735 device datasheet](https://www.cypress.com/documentation/datasheets/cyw20735b1-single-chip-bluetooth-transceiver-wireless-input-devices)
Development kits | Visit https://www.cypress.com/products/airoc-bluetooth-le-bluetooth 
Libraries on GitHub  | [btsdk-drivers](https://github.com/Infineon/btsdk-drivers) – Bluetooth&reg; SDK drivers library <br> [btsdk-mesh](https://github.com/Infineon/btsdk-mesh) – Bluetooth&reg; LE mesh libraries <br> [btsdk-ota](https://github.com/Infineon/btsdk-ota) – Bluetooth&reg; LE OTA libraries <br> [btsdk-ble](https://github.com/Infineon/btsdk-ble) – Bluetooth&reg; LE profile libraries
Tools  | [Eclipse IDE for ModusToolbox&trade; software](https://www.cypress.com/modustoolbox) – ModusToolbox&trade; software is a collection of easy-to-use software and tools enabling rapid development with Infineon MCUs, covering applications from embedded sense and control to wireless and cloud-connected systems using AIROC&trade; Wi-Fi and Bluetooth® connectivity devices. <br> [btsdk-utils](https://github.com/Infineon/btsdk-utils) – Bluetooth&reg; SDK utilities <br> [btsdk-peer-apps-ota](https://github.com/Infineon/btsdk-peer-apps-ota) – Bluetooth&reg; LE OTA peer applications <br> [btsdk-host-peer-apps](https://github.com/Infineon/btsdk-host-peer-apps-mesh) – Bluetooth&reg; LE Mesh Host and peer applications <br> [btsdk-host-apps-bt-ble](https://github.com/Infineon/btsdk-host-apps-bt-ble) – Bluetooth&reg; and Bluetooth&reg; LE Host applications

<br>

## Other resources


Cypress provides a wealth of data at www.cypress.com to help you select the right device, and quickly and effectively integrate it into your design.

## Document history

Document title: *CE228751* - *Bluetooth&reg; LE Peripheral Privacy*

 Version | Description of change
 ------- | ---------------------
 1.0.0   | New code example
 1.1.0   | Updated to support ModusToolbox&trade; software v2.1
 2.0.0   | Major update to support ModusToolbox&trade; software v2.2, added support for new kits <br> This version is not backward compatible with ModusToolbox software v2.1
 2.1.0   | Updated to support ModusToolbox&trade; software v2.3.1 and BTSDK 3.1

<br>


---------------------------------------------------------

© Cypress Semiconductor Corporation, 2020-2021. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates ("Cypress").  This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress’s patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br>
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  No computing device can be absolutely secure.  Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product. CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach").  Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach.  In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications. To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document. Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes.  It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product.  "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage.  Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices.  "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness.  Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device. You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device. Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress’s published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br>
Cypress, the Cypress logo, and combinations thereof, WICED, ModusToolbox, PSoC, CapSense, EZ-USB, F-RAM, and Traveo are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries. For a more complete list of Cypress trademarks, visit cypress.com. Other names and brands may be claimed as property of their respective owners.
