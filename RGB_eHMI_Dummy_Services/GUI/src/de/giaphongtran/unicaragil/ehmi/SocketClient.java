package de.giaphongtran.unicaragil.ehmi;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.Socket;


public class SocketClient {
	private Socket socket;
	private boolean connected;
	
	private BufferedReader input;
	private DataOutputStream output;
	
	public void connect(Server server) {
		try {
			// Open Socket with Server
			socket = new Socket(InetAddress.getByName(server.address), server.port);
			
			// Open input and output streams
			input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			output = new DataOutputStream(socket.getOutputStream());
			
			if (socket.isConnected()) {
				connected = true;
			} else {
				connected = false;
			}
		} catch (IOException e) {}
	}
	
	public String receiveInput() throws IOException {
		String data = input.readLine();
		
		if (data == null) {
			connected = false;
		}
		
		return data;
	}
	
	public void sendCommand(Command command) {
		try {
			output.writeBytes(command.toString() + "\n");
			System.out.println("command sent");
		} catch (IOException e) {}
	}
	
	public void send(String msg) {
		try {
			output.writeBytes(msg);
		} catch (IOException e) {}
	}
	
	public boolean isConnected() {
		if (socket == null) {
			return false;
		} else {
			return connected;
		}
	}
	
	public void disconnect() {		
		// Close streams
		close();
		
		connected = false;
	}
	
	private void close() {
		try {
			if (socket != null) socket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		try {
			if (input != null)  input.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		try {
			if (output != null) output.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
