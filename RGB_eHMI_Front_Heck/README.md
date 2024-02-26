**Was beinhaltet dieser Unterordner?**

Dieser Unterordner enthält den eHMI-Dienst und die dafür notwendigen Bilder und GIFs im .stream Format. Front-Service und Back-Service können auf dem Raspberry Pi kompiliert werden und dann ausgeführt werden.

Hinweis: Front- und Back-Service wurden noch nicht am Fahrzeug getestet.

**Wie ist der Dienst aufgebaut?**

**- eHMI_Symbols**

Beinhaltet die Bilder und GIFs des Projekts zur Darstellung von Informationen für Verkehrsteilnehmer. Diese sind in einem weiteren Unterordner als .stream-Files gespeichert. Diese lassen sich schneller laden auf dem Raspberry Pi.

**- include**

Beinhaltet IDL-Files und Header-Files, die für den ASOA-Dienst relevant sind. Zudem beinhaltet das relevante Files für die RGB-Matrix Library von hzeller ([hzeller RGB-Matrix Library](https://github.com/hzeller/rpi-rgb-led-matrix)).

**- lib**

Beinhaltet Files der hzeller-Library.

**- src**

Beinhaltet die C++ Programme die von der Zielhardware ausgeführt werden sollen (Back_Service.cpp und Front_Service.cpp).

**- CMakeLists.txt**

CMakeLists.txt ist ein notwendiges Textdokument zum Kompilieren des Dienstes. Es bindet zudem die verwendete hzeller-Library und auch die dafür notwendige Magick++ Library ein.

**- Orchestrator-Files**

Sind notwendig, um Dienste zu verbinden und Orchestrator-Anweisungen durchzuführen. Dieses orchestrator-File verbindet dabei den Front-Service mit dem Back-Service.

