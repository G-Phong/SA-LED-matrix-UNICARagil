package de.giaphongtran.unicaragil.ehmi;

import java.awt.EventQueue;

public class Main {

	public static void main(String[] args) {
		
		// Create SocketClient
		SocketClient socketClient = new SocketClient();
		
		// Create the Controller
		Controller controller = new Controller(socketClient);
		
		// Open the GUI
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					GUI frame = new GUI(controller);
					frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
}
