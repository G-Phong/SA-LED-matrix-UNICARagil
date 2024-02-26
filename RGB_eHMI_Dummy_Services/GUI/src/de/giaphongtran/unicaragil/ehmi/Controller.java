package de.giaphongtran.unicaragil.ehmi;

import java.io.IOException;

import org.json.JSONException;
import org.json.JSONObject;

public class Controller {
//	localhost: Localhost ist der Name des virtuellen Servers,
//	der auf dem eigenen Computer läuft.
//	Die IP-Adresse 127.0.0.1 wird benutzt, um über die Loopback Schnittstelle auf den localhost zuzugreifen.
	
	private Server defaultServer = new Server("localhost", 13337);
	SocketClient socketClient;
	
	public boolean fahrzeug_distanz = false;
	public boolean pedCross_distanz = false;
	public boolean Wende_rechts_geplant_in = false;
	public boolean Wende_links_geplant_in = false;
	
	public boolean wendeBeendet = false;
	public boolean parkenAbbrechen = false;
	public boolean parkenBeendet = false;
	
	// DataPublishers
	public DataPublisher<Server> server;
	public DataPublisher<ConnectionStatus> connection_status;
	
	public DataPublisher<Boolean> loading_status;
	public DataPublisher<Boolean> start_right_status;
	public DataPublisher<Boolean> start_left_status;
	public DataPublisher<Boolean> allow_pass_status;
	public DataPublisher<Boolean> ped_cross_status;
	public DataPublisher<Boolean> safe_stop_status;
	public DataPublisher<Boolean> turning_right_status;
	public DataPublisher<Boolean> turning_left_status;
	public DataPublisher<Boolean> stop_right_status;
	public DataPublisher<Boolean> stop_left_status;
	
	public DataPublisher<Boolean> turning_finished_status;
	public DataPublisher<Boolean> parking_aborted_status;
	public DataPublisher<Boolean> parking_finished_status;
	
	public DataPublisher<Float> GUI_time_distance_pedestrian;
	public DataPublisher<Float> GUI_distance_pedestrian;
	public DataPublisher<Float> GUI_distance_passing_vehicle;
	public DataPublisher<Float> GUI_distance_narrowing;
	public DataPublisher<Float> GUI_turning_left_dynamic_module_planned_t;
	public DataPublisher<Float> GUI_turning_right_dynamic_module_planned_t;
	
	public Controller(SocketClient socketClient) {
		this.socketClient = socketClient;
		
		// Initialize DataPublishers
		server = new DataPublisher<Server>(defaultServer);
		connection_status = new DataPublisher<ConnectionStatus>(ConnectionStatus.DISCONNECTED);
		
		loading_status = new DataPublisher<Boolean>(false);
		start_right_status = new DataPublisher<Boolean>(false);
		start_left_status = new DataPublisher<Boolean>(false);
		allow_pass_status = new DataPublisher<Boolean>(false);
		ped_cross_status = new DataPublisher<Boolean>(false);
		safe_stop_status = new DataPublisher<Boolean>(false);
		turning_right_status = new DataPublisher<Boolean>(false);
		turning_left_status = new DataPublisher<Boolean>(false);
		stop_right_status = new DataPublisher<Boolean>(false);
		stop_left_status = new DataPublisher<Boolean>(false);
		
		turning_finished_status = new DataPublisher<Boolean>(false);
		parking_aborted_status = new DataPublisher<Boolean>(false);
		parking_finished_status = new DataPublisher<Boolean>(false);
		
		GUI_time_distance_pedestrian = new DataPublisher<Float>((float) 0.0);
		GUI_distance_pedestrian = new DataPublisher<Float>((float) 0.0);
		GUI_distance_passing_vehicle = new DataPublisher<Float>((float) 0.0);
		GUI_distance_narrowing = new DataPublisher<Float>((float) 0.0);
		GUI_turning_left_dynamic_module_planned_t = new DataPublisher<Float>((float) 0.0);
		GUI_turning_right_dynamic_module_planned_t = new DataPublisher<Float>((float) 0.0);
		
		
		// Connect to the server
		connectToServer();
		
		// Start update Thread
		new Thread(new Runnable() {

			@Override
			public void run() {
				while (true) {
					if (socketClient.isConnected()) {
						
						try {
							// Read input from server
							String input = socketClient.receiveInput();
						//	System.out.println("Server: " + input);
										
							if (input == null) {
								connection_status.setDataSmartNotify(ConnectionStatus.DISCONNECTED);
								continue;
							}
							
							// Parse JSON (if possible)
							JSONObject json = null;
							try {
								json = new JSONObject(input);
							} catch (JSONException e) {}
							
							// Check whether the input was parseable json
							if (json != null) {
								// Get data from json
								
								float GUI_time_distance_pedestrian_json = json.getFloat("GUI_time_distance_pedestrian");
								float GUI_distance_pedestrian_json = json.getFloat("GUI_distance_pedestrian");
								float GUI_distance_passing_vehicle_json = json.getFloat("GUI_distance_passing_vehicle");
								float GUI_distance_narrowing_json = json.getFloat("GUI_distance_narrowing");
								float GUI_turning_left_dynamic_module_planned_t_json = json.getFloat("GUI_turning_left_dynamic_module_planned_t");
								float GUI_turning_right_dynamic_module_planned_t_json = json.getFloat("GUI_turning_right_dynamic_module_planned_t");
								
//								boolean brake_light_status_json = json.getBoolean("brake_light_status");
//								boolean reverse_light_status_json = json.getBoolean("reverse_light_status");
								
								// Update DataPublishers
								GUI_time_distance_pedestrian.setDataSmartNotify(GUI_time_distance_pedestrian_json);
								GUI_distance_pedestrian.setDataSmartNotify(GUI_distance_pedestrian_json);
								GUI_distance_passing_vehicle.setDataSmartNotify(GUI_distance_passing_vehicle_json);
								GUI_distance_narrowing.setDataSmartNotify(GUI_distance_narrowing_json);
								GUI_turning_left_dynamic_module_planned_t.setDataSmartNotify(GUI_turning_left_dynamic_module_planned_t_json);
								GUI_turning_right_dynamic_module_planned_t.setDataSmartNotify(GUI_turning_right_dynamic_module_planned_t_json);
								
							
							}
						} catch (IOException e) {}
						
					} else {
						try {
							Thread.sleep(100);
						} catch (InterruptedException e) {}
					}
				}
			}
		}).start();
	}
	
	public void connectToServer() {
		if (socketClient.isConnected()) {
			return; //wenn es bereits verbunden ist, dann muss nichts mehr gemacht werden
		}
		
		new Thread(new Runnable() {

			@Override
			public void run() {
				// Connect to the server
				connection_status.setDataSmartNotify(ConnectionStatus.CONNECTING);
				socketClient.connect(server.getData());
				
				// Update connectionStatus
				if (socketClient.isConnected()) {
					connection_status.setDataSmartNotify(ConnectionStatus.CONNECTED);
				} else {
					connection_status.setDataSmartNotify(ConnectionStatus.DISCONNECTED);
				}
			}
		}).start();
	}
	
	public void disconnectFromServer() {
		if (!socketClient.isConnected()) {
			return;
		}
		
		connection_status.setDataSmartNotify(ConnectionStatus.DISCONNECTED);
		socketClient.disconnect();
	}
	
	public void setServerAddress(String address) {
		Server oldServer = server.getData();
		server.setDataSmartNotify(new Server(address, oldServer.port));
	}
	
	public void setServerPort(int port) {
		Server oldServer = server.getData();
		server.setDataSmartNotify(new Server(oldServer.address, port));
	}
	
	
	//folgende Toggle-Methoden schicken Commands der GUI an den Server
	public void toggleLoading() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.FAHRZEUG_WIRD_GELADEN);
			loading_status.setData(!loading_status.getData());
		}
	}
	
	public void toggleLParkRight() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.AUSPARKEN_RECHTS);
			start_right_status.setData(!start_right_status.getData());
		}
	}
	
	public void toggleLParkLeft() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.AUSPARKEN_LINKS);
			start_left_status.setData(!start_left_status.getData());
		}
	}
	
	public void toggleAllowPass() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.ENGSTELLE_BEIDSEITIG);
			allow_pass_status.setData(!allow_pass_status.getData());
		}
	}
	
	public void toggleAllowPass_distance() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.ENGSTELLE_BEIDSEITIG_DISTANZ); //TODO: Distanz 100m runterzählen
		}
	}
	
	public void togglePedCross() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.ZEBRASTREIFEN);
			ped_cross_status.setData(!ped_cross_status.getData());
		}
	}
	
	public void togglePedCrossDistance() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.ZEBRASTREIFEN_DISTANZ); //TODO: 10m runterzählen
		}
	}
	
	public void toggleSafeStop() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.EMERGENCY);
			safe_stop_status.setData(!safe_stop_status.getData());
		}
	}
	
	public void toggleTurnRight() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.WENDE_RECHTS);
			turning_right_status.setData(!turning_right_status.getData());
		}
	}
	
	public void toggleTurnRightPlannedIn() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.WENDE_RECHTS_GEPLANT); //TODO: 10s runterzählen
		}
	}
	
	
	public void toggleTurnLeft() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.WENDE_LINKS);
			turning_left_status.setData(!turning_left_status.getData());
		}
	}
	
	public void toggleTurnLeftPlannedIn() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.WENDE_LINKS_GEPLANT); //TODO: 10s runterzählen
		}
	}
	
	public void toggleTurningFinished() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.WENDE_BEENDET);
			turning_finished_status.setData(!turning_finished_status.getData());
		}
	}
	
	
	public void toggleParkRight() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.EINPARKEN_RECHTS);
			stop_right_status.setData(!stop_right_status.getData());
		}
	}
	
	public void toggleParkLeft() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.EINPARKEN_LINKS);
			stop_left_status.setData(!stop_left_status.getData());
		}
	}
	
	public void toggleParkingFinished() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.EINPARKEN_FERTIG);
			parking_finished_status.setData(!parking_finished_status.getData());
		}
	}
	
	public void toggleParkingAborted() {
		if (socketClient.isConnected()) {
			socketClient.sendCommand(Command.EINPARKEN_ABBRECHEN);
			parking_aborted_status.setData(!parking_aborted_status.getData());
		}
	}
	
}
