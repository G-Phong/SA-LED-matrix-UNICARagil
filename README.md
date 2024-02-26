# UNICARagil Semesterarbeit: LED-Matrix Steuerung mit Raspberry Pi
Dies ist mein persönliches Repository für meine Semesterarbeit im UNICARagil-Projekt.

**Um was geht es in diesem Projekt?**

Das Forschungsprojekt UNICARagil verfolgt das Ziel eine autonom fahrende Fahrzeugflotte zu entwickeln. Das vom Bundesministerium für Bildung und Forschung geförderte Projekt vereint dabei Kompetenzen aus führenden deutschen Universitäten im Bereich des autonomen Fahrens. Das Ziel ist die Entwicklung leistungsfähiger, zuverlässiger und emissionsfreier Fahrzeuge.

**Was die Aufgabenstellung dieser Semesterarbeit?**

Die Interaktion mit anderen Verkehrsteilnehmern mit den Fahrzeugen ist ein wichtiger Aspekt, die im Rahmen dieses Projekts über die externe Kommunikation realisiert wird. Die externe Kommunikation kann sowohl die Verkehrseffizienz als auch die Verkehrssicherheit erhöhen und kann zudem die Kooperation im Verkehr ermöglichen (vgl. Schwiebacher, 2021).
Dabei beschäftigt sich diese Semesterarbeit mit dem sogenannten external Human-Machine Interface (kurz: eHMI), die eine Form der externen Kommunikation darstellt.
Konkret soll für das eHMI der Fahrzeuge jeweils eine LED-Matrix in der Front und im Heck verwendet werden. Ziel dieser Arbeit ist es, die LED-Matrizen mit dem Microcontroller Raspberry Pi anzusteuern. Die für den reibungslosen Verkehrsablauf relevanten
Informationen sollen auf den LED-Matrizen dargestellt werden. Für die Vermittlung der
Informationen soll eine Symbolik verwendet werden, die erkennbar, verständlich und für
jeden interpretierbar ist, inklusive Analphabeten oder Kinder. Die Funktionen und Zustände der LED-Matrizen werden durch ein bereits existierendes UML-Zustandsdiagramm beschrieben. 
Der Kern der Arbeit liegt in der Implementierung dieses Zustandsdiagrammes am Raspberry Pi.

**Was enthält dieses Repository?**

* PowerPoint Slides 

&nbsp;
Dieser Ordner enthält PowerPoint Folien, die während der Absprache-Meetings mit Johannes Schwiebacher verwendet wurden. Die Folien sind für die eHMI-Software nicht relevant und können als zusätzliche informelle Informationsquelle gesehen werden. Die Richtigkeit der Folien ist jedoch nicht gewährleistet, diese dienen eher der Aufzeichnung von Gedanken während der Arbeit.

* RGB_eHMI_Dummy_Services 

&nbsp;
Enthält den Dummy-Service (ASOA-Dienst) und die GUI zum Testen. Der Dummy-Service simuliert Dienste, die während dem Realbetrieb mit dem Front- und Back-Service kommunizieren sollen. Mit Hilfe der GUI lassen sich Fahrzeugzustände per Knopfdruck simulieren.

* RGB_eHMI_Front_Heck 

&nbsp;
Enthält den Front- und Back-Service. Beide Dienste wurden noch nicht am Fahrzeug erprobt. [Stand: 09.11.2021]

* UML Diagramme autoTAXI 

&nbsp;
Enthält die UML-Zustandsdiagramme für das autonome Fahrzeug im PDF-Format.

**Weiterführende Informationen so wie Anleitungen zur Bedienung befinden sich in den entsprechenden Unterordnern!**

Bei Fragen kann gerne eine E-Mail an *Gia-Phong Tran* (ge79kis@tum.de) geschrieben werden.


Hinweis: Dieses Projekt nutzt die RGB-Matrix Library (c) Henner Zeller h.zeller@acm.org ([hzeller RGB-Matrix Library](https://github.com/hzeller/rpi-rgb-led-matrix)). Die Library ist mit der **GNU General Public License Version 2.0** lizenziert.
