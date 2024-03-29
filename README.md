# Bluetooth&reg; LE peripheral privacy

This code example demonstrates the privacy features available to users in Bluetooth&reg; 5.0 and above using ModusToolbox™ software.

Features demonstrated:
1. Privacy modes as defined in Bluetooth&reg; spec 5.0 and above.
2. Use of persistent storage for bond data management.
3. Management and handling of bond data of multiple peer devices.

[View this README on GitHub.](https://github.com/Infineon/mtb-example-btsdk-peripheral-privacy)

[Provide feedback on this code example.](https://cypress.co1.qualtrics.com/jfe/form/SV_1NTns53sK2yiljn?Q_EED=eyJVbmlxdWUgRG9jIElkIjoiQ0UyMjg3NTEiLCJTcGVjIE51bWJlciI6IjAwMi0yODc1MSIsIkRvYyBUaXRsZSI6IkJsdWV0b290aCZyZWc7IExFIHBlcmlwaGVyYWwgcHJpdmFjeSIsInJpZCI6ImFtbWwiLCJEb2MgdmVyc2lvbiI6IjIuMy4wIiwiRG9jIExhbmd1YWdlIjoiRW5nbGlzaCIsIkRvYyBEaXZpc2lvbiI6Ik1DRCIsIkRvYyBCVSI6IklDVyIsIkRvYyBGYW1pbHkiOiJCVEFCTEUifQ==)


## Requirements

- [ModusToolbox&trade; software](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software/) v2.4 or later (tested with v2.4)

- Board support package (BSP) minimum required version: 3.0.0
- Programming language: C

- Associated parts:  [AIROC™ CYW20819 Bluetooth® & Bluetooth® LE system-on-chip](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-bluetooth-le-bluetooth-multiprotocol/cyw20819/), [AIROC™ CYW20719 Bluetooth® & Bluetooth® LE system-on-chip](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-bluetooth-le-bluetooth-multiprotocol/cyw20719/), [AIROC™ CYW20820 Bluetooth® & Bluetooth® LE system-on-chip](https://www.infineon.com/dgdl/Infineon-CYW20820_Ultra_Low_Power_Bluetooth_LE_BR_EDR_Bluetooth_5.0_SoC-AdditionalTechnicalInformation-v06_00-EN.pdf?fileId=8ac78c8c7d0d8da4017d0ee7e70770d1), [AIROC™ CYW20835 Bluetooth® & Bluetooth® LE system-on-chip](https://www.infineon.com/dgdl/Infineon-AIROC_CYW20835_Bluetooth_LE_System-on-chip-AdditionalTechnicalInformation-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d945f48bd4c3b)

## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm® embedded compiler v9.3.1 (`GCC_ARM`) - Default value of `TOOLCHAIN`

## Supported kits (make variable 'TARGET')
- [CYW920820M2EVB-01 evaluation kit](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-bluetooth-le-bluetooth-multiprotocol/cyw20820/) (`CYW920820M2EVB-01`)- Default value of `TARGET`
- [CYW920835M2EVB-01 evaluation kit](https://www.infineon.com/cms/en/product/evaluation-boards/cyw920835m2evb-01) (`CYW920835M2EVB-01`)
-  [CYW920819M2EVB-01 evaluation kit](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-bluetooth-le-bluetooth-multiprotocol/cyw20819/)(`CYW920819M2EVB-01`)
- [CYW989820M2EVB-01 evaluation kit](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-bluetooth-le-bluetooth-multiprotocol/cyw89820/)(`CYW989820M2EVB-01`)
- [CYW920819EVB-02 evaluation kit](https://www.infineon.com/cms/en/product/evaluation-boards/cyw920819evb-02) (`CYW920819EVB-02`) 
- [CYW920820EVB-02 evaluation kit](https://www.infineon.com/cms/en/design-support/finder-selection-tools/product-finder/evaluation-board/?redirId=59313) (`CYW920820EVB-02`)
- [CYW920719B2Q40EVB-01 evaluation kit](https://www.infineon.com/cms/en/product/evaluation-boards/cyw920719b2q40evb-01/) (`CYW920719B2Q40EVB-01`)


## Hardware setup

This example uses the board's default configuration. See the kit user guide to ensure that the board is configured correctly.


## Software setup

Install a terminal emulator if you don't have one. Instructions in this document use [Tera Term](https://ttssh2.osdn.jp/index.html.en). All other required software comes bundled with the ModusToolbox&trade; software.


## Using the code example

Create the project and open it using one of the following:

<details><summary><b>In Eclipse IDE for ModusToolbox&trade; software</b></summary>

1. Click the **New Application** link in the **Quick Panel** (or, use **File** > **New** > **ModusToolbox Application**). This launches the [Project Creator](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Project_Creator_Guide_3-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99bcabbd31e5) tool.

2. Pick a kit supported by the code example from the list shown in the **Project Creator - Choose Board Support Package (BSP)** dialog.

   When you select a supported kit, the example is reconfigured automatically to work with the kit. To work with a different supported kit later, use the [Library Manager](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Library_Manager_User_Guide_3-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99ab34b831ce) to choose the BSP for the supported kit. You can use the Library Manager to select or update the BSP and firmware libraries used in this application. To access the Library Manager, click the link from the **Quick Panel**.

   You can also just start the application creation process again and select a different kit.

   If you want to use the application for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.
3. In the **Project Creator - Select Application** dialog, choose the **Bluetooth&reg;LE_Peripheral_Privacy** application by enabling the checkbox.

4. (Optional) Change the suggested **New Application Name**.

5. The **Application(s) Root Path** defaults to the Eclipse workspace which is usually the desired location for the application. If you want to store the application in a different location, you can change the *Application(s) Root Path* value. Applications that share libraries should be in the same root path.

6. Click **Create** to complete the application creation process.

For more details, see the [Eclipse IDE for ModusToolbox&trade; software user guide](https://www.infineon.com/dgdl/Infineon-Eclipse_IDE_for_ModusToolbox_User_Guide_1-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99bcb86331e8) (locally available at *{ModusToolbox&trade; software install directory}/ide_{version}/docs/mt_ide_user_guide.pdf*).

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

The following example will clone the "[BTSDK Peripheral Privacy](https://github.com/Infineon/mtb-example-btsdk-peripheral-privacy)" application with the desired name "BTSDKPeripheralPrivacy" configured for the *CYW920820M2EVB-01* BSP into the specified working directory, *C:/mtb_projects*:

   ```
   project-creator-cli --board-id CYW920820M2EVB-01 --app-id mtb-example-btsdk-peripheral-privacy --user-app-name BTSDKPeripheralPrivacy --target-dir "C:/mtb_projects"
   ```

**Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For details, see the "Project creator tools" section of the [ModusToolbox&trade; software user guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_2.4_User_Guide-Software-v01_00-EN.pdf?fileId=8ac78c8c7e7124d1017ed97e72563632) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>

<details><summary><b>In third-party IDEs</b></summary>

Use one of the following options:

- **Use the standalone [Project Creator](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Project_Creator_Guide_3-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99bcabbd31e5) tool:**

   1. Launch Project Creator from the Windows Start menu or from *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/project-creator.exe*.

   2. In the initial **Choose Board Support Package** screen, select the BSP, and click **Next**.

   3. In the **Select Application** screen, select the appropriate IDE from the **Target IDE** drop-down menu.

   4. Click **Create** and follow the instructions printed in the bottom pane to import or open the exported project in the respective IDE.

<br>

- **Use command-line interface (CLI):**

   1. Follow the instructions from the **In command-line interface (CLI)** section to create the application, and then import the libraries using the `make getlibs` command.

   2. Export the application to a supported IDE using the `make <ide>` command.

   3. Follow the instructions displayed in the terminal to create or import the application as an IDE project.

For a list of supported IDEs and more details, see the "Exporting to IDEs" section of the [ModusToolbox&trade; software user guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_2.4_User_Guide-Software-v01_00-EN.pdf?fileId=8ac78c8c7e7124d1017ed97e72563632) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>


## Operation

1. Connect the board to your PC using the provided USB cable through the USB connector.

2. Open a terminal program and select the COM port. Set the serial port parameters to 8N1 and 115200 baud.

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

4. The application runs a custom button service with one custom characteristic that counts the number of button presses on the kit. It can be read or setup for notifications. Each time the button on the kit is pressed, the count value is incremented. If any device is connected and has notifications enabled, the updated value is sent to it. If no device is connected or notifications are disabled a message informing the same is displayed.

  **NOTE:** The button count is incremented on the button press irrespective of whether any device is connected or not.

5. Following instructions appear on the terminal on application start:
    * Press **'l'** to check for the number of bonded devices and next empty slot
        - This option allows you to see how many devices are paired to the peripheral and which is the next available slot. This example supports up to four bonded devices after which the oldest devices data is overwritten.
    * Press **'r'** to erase all the bond data present in NVRAM
        - This option allows user to clear the memory of all the present bond data.
    * Press **'e'** to enter the bonding mode and add devices to bond list
        - This option puts the peripheral into bonding mode allowing it to connect and bond with new devices. After connection and bonding, the incoming device can read and subscribe to the custom button count service.
    * Press **'p'** to change privacy mode of bonded device
        - This is used to change the privacy mode setting of the bonded devices i.e., to move devices from device privacy mode to netwok privacy mode and vice versa. For more information about the privacy modes, read the [design and implementation](#design-and-implementation) section.
    * Press **'h'** any time in application to print the menu
        - At any point in the program to look at the available options, user can request the start menu options.

        Use these available commands to interact with the application. Refer [Figure 2](#Figure-2-Process-Flowchart) for the application flow chart.


## Design and implementation

The objective of the application is to demonstrate how the user can make use of privacy features available in the Bluetooth&reg; devices. 

Bluetooth&reg; devices implement privacy mainly by using **different types of addresses**: public, which means no privacy, or random. Random addresses can be static (which don't change) or private (which change periodically, offering privacy protection). Private addresses can be further divided into non-resolvable or resolvable.

The use of resolvable private addresses allows only the devices that are paired to your device to "identify" the device as a known device; all other devices will perceive the device as a new device, making it difficult to track. If the device uses non-resolvable private address, it will be perceived as a new device every time it changes its address. The address is changed at regular intervals and is configurable.

Every privacy-enabled Bluetooth&reg;LE device has a unique address called the identity address and an identity resolving key (IRK). The identity address is the public address or static address of the Bluetooth&reg;LE device. The IRK is used by the BLE device to generate its resolvable private address (RPA) and is used by peer devices to resolve the RPA of the Bluetooth&reg; LE device. Both the identity address and the IRK are exchanged during the pairing process. Privacy-enabled Bluetooth&reg; LE devices maintain a list that consists of the peer device’s identity address, the local IRK used by the Bluetooth&reg; LE device to generate its RPA, and the peer device’s IRK used to resolve the peer device’s RPA. This is called the resolving list. Only peer devices that have the 128-bit identity resolving key (IRK) of a Bluetooth&reg; LE device can determine the device's address.

Maximum resolving list size supported on different devices is:
- CYW20819    : 20 entries
- CYW20820    : 20 entries
- CYW20735B1  : 128 entries
- CYW20719B2  : 128 entries

   **Figure 1. Bluetooth&reg; LE address types**

   ![](images/address_types.jpg)

Apart from this, Bluetooth&reg; 5.0 introduced more options in the form of **privacy modes**.

There are two modes of privacy: **device privacy** mode and **network privacy** mode.

A device in **device privacy** mode is only concerned about the privacy of the device and will accept advertising/connection packets from peer devices that contain their identity address and a private address, even if the peer device has distributed its identity resolution key (IRK) in the past.

In **network privacy** mode, a device will only accept advertising/connection packets from peer devices that contain private addresses. By default, network privacy mode is used when private addresses are resolved and generated by the controller.

[Table 1](#table-1-logical-representation-of-resolving-list-entries) shows the logical representation of the resolving list when a device that is trying to reconnect the controller checks the resolving list for the device. Depending on the result two things can happen as follows:

1. Device is found in the list.

Advertisement/Connection address type| Privacy mode  |Request accepted/rejected
-------------------------------------|---------------|-------------------
Identity address | Network| Rejected
Identity address | Device | Accepted
Resolvable private address| Network| Accepted
 Resolvable private address| Device| Accepted 

2. Device is not found in the list.

    In this case, the incoming device is treated as a new device and the request is forwarded to the host by the controller for further processing.

**Note:** A device using non-resolvable private address will be treated as a new device on every reconnection.


 **Table 1. Logical representation of resolving list entries**

Device| Local IRK| Peer IRK| Peer identity address|Identity address type| Privacy mode
------|----------|---------|----------------------|---------------------|-------------
1     |Local IRK | Peer 1 IRK| Peer 1 identity address|Static/public| Network/device
2     |Local IRK | Peer 2 IRK| Peer 2 identity address|Static/public| Network/device
3     |Local IRK | Peer 3 IRK| Peer 3 identity address|Static/public| Network/device

The application runs a custom button service with one custom characteristic that counts the number of button presses on the kit. It can be read or setup for notifications. The GATT DB is setup so that the characteristic can be read without pairing/bonding but for enabling and disabling notifications pairing/bonding is required. When the button on the kit is pressed every time, the count value is incremented. If any device is connected and has notifications enabled the updated value is sent to it. If no device is connected or notifications are disabled, a message informing the same is displayed.

The device can store bond data of up to four peer devices after which the data of the oldest device is overwritten by that of the new incoming device. The incoming device is added in device privacy mode by default which can be changed by issuing **'p'** command via terminal.

The peripheral has five states:

1. **IDLE_NO_DATA:** The device in this state is either waiting for the user input or advertising. No bond data is present in the NVRAM. Directed advertising option is disabled in this state.
2. **IDLE_DATA:** The device in this state is either waiting for the user input or advertising. Bond data is present in the NVRAM. Directed advertising option is available.
3. **IDLE_PRIVACY_CHANGE:** The device in this state is not advertising. The device enters this mode when command to change the privacy mode of bonded devices is issued.
4. **CONNECTED:** In this state, the peripheral is connected to a peer device.
5. **BONDED:** The peripheral moves into this state once it has paired and bonded with the connected device and the peer bond information has been saved to NVRAM.

The LED1 on the kit is used to represent the current advertising state of the device. [Table 2](#table-2-led-behaviour-for-advertising-states) shows LED behaviour for different advertising states.

 **Table 2. LED behaviour for advertising states**
 
Advertisement state | LED state
------------------- | ----------
Advertisement ON (undirected): | Slow blinking led (T = 1 sec)
Advertisement ON (directed):  | Fast blinking led (T = 20msec)
Advertisement OFF, connected: | LED ON
Advertisement OFF, timed out: | LED OFF

   **Figure 2. Process flowchart**

   ![](images/process_flow_chart_directed.jpg)

<br>

## Resources and settings

This section explains the ModusToolbox&trade; software resources and their configuration as used in this code example. Note that all the configuration explained in this section has already been done in the code example. Eclipse IDE for ModusToolbox&trade; software stores the configuration settings of the application in the *design.modus* file. This file is used by the graphical configurators, which generate the configuration firmware. This firmware is stored in the application’s *GeneratedSource* folder.

- **Device configurator:** Used to enable/configure the peripherals and the pins used in the application. See the
[Device configurator guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Device_Configurator_Guide_4-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99ab297631cb).

* **Bluetooth&reg; configurator:** Used for generating/modifying the Bluetooth&reg; LE GATT database. See the
[Bluetooth&reg; configurator guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Bluetooth_Configurator_Guide_3-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99aaf5b231be).

<br>

## Related resources

Resources  | Links
-----------| ----------------------------------
Application notes  | [AN225684](https://www.infineon.com/dgdl/Infineon-AN225684_Getting_Started_with_CYW20819-ApplicationNotes-v02_00-EN.PDF?fileId=8ac78c8c7cdc391c017d0d3674d1669d): Getting started with CYW208xx |
Code examples  | [Using ModusToolbox&trade; software](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software) on GitHub <br> [Using Bluetooth&reg; SDK](https://www.infineon.com/cms/en/design-support/software/code-examples/psoc-6-code-examples-for-modustoolbox/bluetooth-sdk-code-examples/)
Device documentation | [CYW20819 device datasheet](https://www.infineon.com/dgdl/Infineon-CYW20819_Ultra_Low_Power_Bluetooth_LE_BR_EDR_Bluetooth_5.0_SoC-AdditionalTechnicalInformation-v07_00-EN.pdf?fileId=8ac78c8c7d0d8da4017d0ee7dba070bf) <br> [CYW20820 device datasheet](https://www.infineon.com/dgdl/Infineon-CYW20820_Ultra_Low_Power_Bluetooth_LE_BR_EDR_Bluetooth_5.0_SoC-AdditionalTechnicalInformation-v06_00-EN.pdf?fileId=8ac78c8c7d0d8da4017d0ee7e70770d1) <br> [CYW20719 device datasheet](https://www.infineon.com/dgdl/Infineon-CYW20719B2KUMLGT-DataSheet-v07_00-EN.pdf?fileId=8ac78c8c7d0d8da4017d0ee7edf470db) <br>[CYW89820 device documents](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-bluetooth-le-bluetooth-multiprotocol/cyw89820/#!documents)
Development kits | Visit https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-bluetooth-le-bluetooth-multiprotocol 
Libraries on GitHub  | [btsdk-drivers](https://github.com/Infineon/btsdk-drivers) – Bluetooth&reg; SDK drivers library <br> [btsdk-mesh](https://github.com/Infineon/btsdk-mesh) – Bluetooth&reg; LE mesh libraries <br> [btsdk-ota](https://github.com/Infineon/btsdk-ota) – Bluetooth&reg; LE OTA libraries <br> [btsdk-ble](https://github.com/Infineon/btsdk-ble) – Bluetooth&reg; LE profile libraries
Tools  | [Eclipse IDE for ModusToolbox&trade; software](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software/) – ModusToolbox&trade; software is a collection of easy-to-use software and tools enabling rapid development with Infineon MCUs, covering applications from embedded sense and control to wireless and cloud-connected systems using AIROC&trade; Wi-Fi and Bluetooth® connectivity devices. <br> [btsdk-utils](https://github.com/Infineon/btsdk-utils) – Bluetooth&reg; SDK utilities <br> [btsdk-peer-apps-ota](https://github.com/Infineon/btsdk-peer-apps-ota) – Bluetooth&reg; LE OTA peer applications <br> [btsdk-host-peer-apps](https://github.com/Infineon/btsdk-host-peer-apps-mesh) – Bluetooth&reg; LE mesh host and peer applications <br> [btsdk-host-apps-bt-ble](https://github.com/Infineon/btsdk-host-apps-bt-ble) – Bluetooth&reg; and Bluetooth&reg; LE host applications

<br>

## Other resources


Infineon provides a wealth of data at www.infineon.com to help you select the right device, and quickly and effectively integrate it into your design.

## Document history

Document title: *CE228751* - *Bluetooth&reg; LE peripheral privacy*

 Version | Description of change
 ------- | ---------------------
 1.0.0   | New code example
 1.1.0   | Updated to support ModusToolbox&trade; software v2.1
 2.0.0   | Major update to support ModusToolbox&trade; software v2.2, added support for new kits <br> This version is not backward compatible with ModusToolbox&trade; software v2.1
 2.1.0   | Updated to support ModusToolbox&trade; software v2.3.1 and BTSDK 3.1
 2.2.0   | Added support for 20820 and 20835 kit
 2.3.0   | Added support for CYW920819M2EVB-01 and CYW989820M2EVB-01

<br>


---------------------------------------------------------

© Cypress Semiconductor Corporation, 2020-2022. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates ("Cypress").  This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress’s patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br>
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  No computing device can be absolutely secure.  Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product. CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach").  Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach.  In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications. To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document. Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes.  It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product.  "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage.  Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices.  "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness.  Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device. You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device. Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress’s published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br>
Cypress, the Cypress logo, and combinations thereof, WICED, ModusToolbox, PSoC, CapSense, EZ-USB, F-RAM, and Traveo are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries. For a more complete list of Cypress trademarks, visit cypress.com. Other names and brands may be claimed as property of their respective owners.
