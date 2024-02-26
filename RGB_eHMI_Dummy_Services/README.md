**Was beinhaltet dieser Unterordner?**

Dieser Unterordner enthält den Dummy-Dienst für den eHMI-Dienst. Der Dummy Dienst testet die Funktionalität des eHMI-Dienstes (also Front- und Back-Service). Zusätzlich ist eine Dummy-Version des Back-Services gegeben, die statt der Ausgabe auf der LED-Matrix nur auf die Konsole druckt (Testzwecke).

Beim Testen werden zunächst der Dummy_Service und der Back-Service kompiliert. Beide Programme werden dann auf der virtuellen Maschine ausgeführt und anschließend mit Hilfe der Orchestrator-Dateien zusammen mit dem Front-Service (der auf dem Raspberry Pi läuft) vernetzt. Dazu muss das lokale Netzwerk (Host-PC, VM und Raspberry Pi) entsprechend eingerichtet sein. Sobald der Orchestrator - der auf der VM läuft - alle Dienste erkennt, so kann die GUI auf der VM gestartet werden (siehe Ordner "GUI") und die Simulation der Fahrzeugzustände kann begonnen werden. Zu erwarten ist dann eine Anzeige auf der LED-Matrix durch den Front-Service und eine Konsolenausgabe des Back-Service. 

**Wie ist der Dienst aufgebaut?**

**- GUI**

Beinhaltet ein in JAVA geschriebenes grafisches Interface, mit dem sich Front- und Back-Service testen lassen. Zur Ausführung der .jar-Datei muss eine Java-Laufzeitumgebung installiert sein. Unter Ubuntu 18 muss zusätzlich unter "Properties" die Eigenschaft "Execute as Runnable" aktiviert sein. Danach lässt sich die GUI per Doppelklick öffnen. 


**- include**

Beinhaltet IDL-Files und Header-Files, die für den ASOA-Dummy-Dienst relevant sind. Zusätzlich enhält dieser Unterordner relevante Files für die Kommunikation über Server- und ClientSockets. Diese werden im Dienst eingebunden.


**- src**

Beinhaltet die C++ Programme die von der Zielhardware ausgeführt werden sollen (Back_Service.cpp und Dummy_Service.cpp)

**- CMakeLists.txt**

CMakeLists.txt ist ein notwendiges Textdokument zum Kompilieren des Dienstes. Hier muss lediglich der ASOA Core eingebunden werden, nicht aber die hzeller Bibliothek.

**- Orchestrator-Files**

Sind notwendig, um Dienste zu verbinden und Orchestrator-Anweisungen durchzuführen. Dieses orchestrator-textfile verbindet den Dummy-Service mit dem Front-Service.

