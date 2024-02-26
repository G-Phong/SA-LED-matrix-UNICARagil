package de.giaphongtran.unicaragil.ehmi;

import java.awt.BorderLayout;
import java.awt.Color;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.border.EmptyBorder;
import javax.swing.JLabel;
import javax.swing.JOptionPane;

import java.awt.Font;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.Image;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import javax.swing.SwingConstants;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.JTextField;

public class GUI extends JFrame {

	
	private Controller controller;
	
	private JPanel contentPane;
	
	// Images
	private Image loadingOn_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Akku_Laden.png")).getImage();
	private Image loadingOff_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Akku_Laden_off.png")).getImage();
	
	private Image safeStopOn_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Ausrufezeichen_Warnung.png")).getImage();
	private Image safeStopOff_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Ausrufezeichen_Warnung_off.png")).getImage();
	
	private Image allowPassOn_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Engstelle.png")).getImage();
	private Image allowPassOff_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Engstelle_off.jpg")).getImage();
	
	private Image parkLeftOn_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Parken_links.png")).getImage();
	private Image parkLeftOff_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Parken_links_off.png")).getImage();
	
	private Image parkRightOn_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Parken_rechts.png")).getImage();
	private Image parkRightOff_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Parken_rechts_off.png")).getImage();
	
	private Image pedCrossOn_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Zebrastreifen.png")).getImage();
	private Image pedCrossOff_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Zebrastreifen_off.png")).getImage();
	
	private Image turningRightOn_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Wendemanöver_mit_UZS_statisch.jpg")).getImage();
	private Image turningRightOff_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Wendemanöver_mit_UZS_statisch_off.jpg")).getImage();
	
	private Image turningLeftOn_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Wendemanöver_gegen_UZS_statisch.jpg")).getImage();
	private Image turningLeftOff_image = new ImageIcon(this.getClass().getClassLoader().getResource("eHMI_symbols/Wendemanöver_gegen_UZS_statisch_off.jpg")).getImage();
	
	
	private ImageIcon loadingOn, loadingOff;
	private ImageIcon safeStopOn, safeStopOff;
	private ImageIcon allowPassOn, allowPassOff;
	private ImageIcon parkLeftOn, parkLeftOff;
	private ImageIcon parkRightOn, parkRightOff;
	private ImageIcon pedCrossOn, pedCrossOff;
	private ImageIcon turningLeftOn, turningLeftOff;
	private ImageIcon turningRightOn, turningRightOff;
	
	private JTextField textField_IP;
	private JTextField textField_Port;
	
	/**
	 * Create the frame.
	 */
	public GUI(Controller controller) {
		this.controller = controller;
		
		//reloadImages();
		
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 800, 560);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		contentPane.setLayout(new BorderLayout(0, 0));
		setContentPane(contentPane);
		
		
		//______________________________________________ top panel for indicators __________________________________________
		
		JPanel topPanel = new JPanel();
		contentPane.add(topPanel, BorderLayout.NORTH);
		topPanel.setLayout(new BorderLayout(0, 0));
		
		JPanel topPanel_west = new JPanel();
		topPanel.add(topPanel_west, BorderLayout.WEST);
		
		JLabel lblConnectionstatus = new JLabel("ConnectionStatus: ");
		lblConnectionstatus.setFont(new Font("Tahoma", Font.PLAIN, 14));
		topPanel_west.add(lblConnectionstatus);
		
		JButton btnReconnect = new JButton("Reconnect");
		btnReconnect.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.connectToServer();
			}
		});
		btnReconnect.setBackground(new Color(200, 220, 255));
		btnReconnect.setOpaque(true);
		topPanel_west.add(btnReconnect);
		
		JButton btnDisconnect = new JButton("Disconnect");
		btnDisconnect.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.disconnectFromServer();
			}
		});
		btnDisconnect.setBackground(new Color(255, 150, 150));
		btnDisconnect.setOpaque(true);
		topPanel_west.add(btnDisconnect);
		
		JPanel topPanel_east = new JPanel();
		topPanel.add(topPanel_east, BorderLayout.EAST);
		
		JLabel lblIP = new JLabel("IP:");
		topPanel_east.add(lblIP);
		
		textField_IP = new JTextField();
		textField_IP.setEditable(false);
		topPanel_east.add(textField_IP);
		textField_IP.setColumns(10);
		
		JButton btnEditIP = new JButton("");
		btnEditIP.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				do {
					String input = JOptionPane.showInputDialog(btnEditIP, "Please input the server ip-address:", "Server ip-address", JOptionPane.PLAIN_MESSAGE);
					
					// Check if input was cancelled
					if (input == null) {
						break;
					}
					
					if (input.isEmpty()) {
						JOptionPane.showMessageDialog(btnEditIP, "Invalid input: empty input", "Error", JOptionPane.ERROR_MESSAGE);
						continue;
					}
					
					// TODO: Validate input is valid server address?
					
					controller.setServerAddress(input);
					break;
				} while(true);
			}
		});
		btnEditIP.setIcon(new ImageIcon(GUI.class.getResource("/images/edit.png")));
		btnEditIP.setToolTipText("Edit server ip");
		btnEditIP.setBorder(BorderFactory.createLineBorder(Color.DARK_GRAY));
		btnEditIP.setOpaque(false);
		btnEditIP.setBackground(Color.GRAY);
		topPanel_east.add(btnEditIP);
		
		JLabel lblPort = new JLabel("Port:");
		topPanel_east.add(lblPort);
		
		textField_Port = new JTextField();
		textField_Port.setEditable(false);
		topPanel_east.add(textField_Port);
		textField_Port.setColumns(5);
		
		JButton btnEditPort = new JButton("");
		btnEditPort.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				do {
					String input = JOptionPane.showInputDialog(btnEditPort, "Please input the server port:", "Server port", JOptionPane.PLAIN_MESSAGE);
					
					// Check if input was cancelled
					if (input == null) {
						break;
					}
					
					// Check if input is numbers only
					if (!input.matches("\\d+")) {
						JOptionPane.showMessageDialog(btnEditPort, "Invalid input: not a number", "Error", JOptionPane.ERROR_MESSAGE);
						continue;
					}
					
					int port = Integer.parseInt(input);
					
					// Validate port is in port-range
					if (port < 0 || port > 65535) {
						JOptionPane.showMessageDialog(btnEditPort, "Invalid input: port is out of valid port range.", "Error", JOptionPane.ERROR_MESSAGE);
						continue;
					}
					
					controller.setServerPort(port);
					break;
				} while(true);
			}
		});
		btnEditPort.setIcon(new ImageIcon(GUI.class.getResource("/images/edit.png")));
		btnEditPort.setToolTipText("Edit server port");
		btnEditPort.setBorder(BorderFactory.createLineBorder(Color.DARK_GRAY));
		btnEditPort.setOpaque(false);
		btnEditPort.setBackground(Color.GRAY);
		topPanel_east.add(btnEditPort);
		
		
		//___________________________________________________ main panel declarations ____________________________________________
		
		JPanel mainPanel = new JPanel();
		contentPane.add(mainPanel, BorderLayout.CENTER);
		mainPanel.setLayout(new BorderLayout(0, 0));
		
		JPanel mainPanel_north = new JPanel();
		mainPanel.add(mainPanel_north, BorderLayout.NORTH);
		
		JPanel mainPanel_west = new JPanel();
		mainPanel.add(mainPanel_west, BorderLayout.WEST);
		mainPanel_west.setLayout(new GridLayout(0, 1, 0, 10));
		
		JPanel mainPanel_center = new JPanel();
		mainPanel.add(mainPanel_center, BorderLayout.CENTER);
		
		JPanel mainPanel_east = new JPanel();
		mainPanel.add(mainPanel_east, BorderLayout.EAST);
		mainPanel_east.setLayout(new GridLayout(0, 1, 0, 10));
		
		JPanel mainPanel_south = new JPanel();
		mainPanel.add(mainPanel_south, BorderLayout.SOUTH);
		
		//MAIN PANEL WEST PANELS:
		//Fahrzeug wird geladen
		JPanel mainPanel_west_loading = new JPanel();
		mainPanel_west.add(mainPanel_west_loading);
		mainPanel_west_loading.setLayout(new GridLayout(1, 0, 0, 0));
		
		JLabel lblLoading = new JLabel("");
		lblLoading.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_west_loading.add(lblLoading);
		
		JButton btnLoading = new JButton("Fahrzeug wird geladen");
		btnLoading.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.toggleLoading();
			}
		});
		btnLoading.setBackground(Color.WHITE);
		btnLoading.setOpaque(true);
		btnLoading.setBorderPainted(false);
		mainPanel_west_loading.add(btnLoading);
		
		//Ausparken rechts
		JPanel mainPanel_west_LParkRight = new JPanel();
		mainPanel_west.add(mainPanel_west_LParkRight);
		mainPanel_west_LParkRight.setLayout(new GridLayout(1, 0, 0, 0));
		
		JLabel lblLParkRight = new JLabel("");
		lblLParkRight.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_west_LParkRight.add(lblLParkRight);
		
		JButton btnLParkRight = new JButton("Ausparken rechts 90°");
		btnLParkRight.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.toggleLParkRight();
			}
		});
		btnLParkRight.setBackground(Color.WHITE);
		btnLParkRight.setOpaque(true);
		btnLParkRight.setBorderPainted(false);
		mainPanel_west_LParkRight.add(btnLParkRight);
		
		//Ausparken links
		JPanel mainPanel_west_LParkLeft = new JPanel();
		mainPanel_west.add(mainPanel_west_LParkLeft);
		mainPanel_west_LParkLeft.setLayout(new GridLayout(1, 0, 0, 0));
		
		JLabel lblLParkLeft = new JLabel("");
		lblLParkLeft.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_west_LParkLeft.add(lblLParkLeft);
		
		JButton btnLParkLeft = new JButton("Ausparken links 90°");
		btnLParkLeft.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.toggleLParkLeft();
			}
		});
		btnLParkLeft.setBackground(Color.WHITE);
		btnLParkLeft.setOpaque(true);
		btnLParkLeft.setBorderPainted(false);
		mainPanel_west_LParkLeft.add(btnLParkLeft);
		
		//Engstelle beidseitig
		JPanel mainPanel_west_allowPass = new JPanel();
		mainPanel_west.add(mainPanel_west_allowPass);
		mainPanel_west_allowPass.setLayout(new GridLayout(1, 0, 0, 0));
		
		JLabel lblAllowPass = new JLabel("");
		lblAllowPass.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_west_allowPass.add(lblAllowPass);
		
		JButton btnAllowPass = new JButton("Engstelle beidseitig");
		btnAllowPass.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.toggleAllowPass();
			}
		});
		btnAllowPass.setBackground(Color.WHITE);
		btnAllowPass.setOpaque(true);
		btnAllowPass.setBorderPainted(false);
		mainPanel_west_allowPass.add(btnAllowPass);
		
		//Engstelle beidseitig: Fahrzeug-Distanz
		JPanel mainPanel_west_allowPass_distance = new JPanel();
		mainPanel_west.add(mainPanel_west_allowPass_distance);
		mainPanel_west_allowPass_distance.setLayout(new GridLayout(1, 0, 0, 0));
		
		JLabel lblAllowPass_distance = new JLabel("");
		lblAllowPass_distance.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_west_allowPass_distance.add(lblAllowPass_distance);
		
		JTextArea allowPass_distance_textArea = new JTextArea();
		allowPass_distance_textArea.setText("TODO: m");
		mainPanel_west_allowPass_distance.add(allowPass_distance_textArea);
		
		JButton btnAllowPass_distance = new JButton("Fahrzeug-Distanz [120m -> 0m]");
		btnAllowPass_distance.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.toggleAllowPass_distance();
				controller.fahrzeug_distanz = !controller.fahrzeug_distanz;
			}
		});
		btnAllowPass_distance.setBackground(Color.cyan);
		btnAllowPass_distance.setOpaque(true);
		btnAllowPass_distance.setBorderPainted(false);
		mainPanel_west_allowPass_distance.add(btnAllowPass_distance);
		
		//Fußgänger an Zebrastreifen
		JPanel mainPanel_west_pedCross = new JPanel();
		mainPanel_west.add(mainPanel_west_pedCross);
		mainPanel_west_pedCross.setLayout(new GridLayout(1, 0, 0, 0));
		
		JLabel lblPedCross = new JLabel("");
		lblPedCross.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_west_pedCross.add(lblPedCross);
		
		JButton btnPedCross = new JButton("Fußgänger an Zebrastreifen");
		btnPedCross.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.togglePedCross();
			}
		});
		btnPedCross.setBackground(Color.WHITE);
		btnPedCross.setOpaque(true);
		btnPedCross.setBorderPainted(false);
		mainPanel_west_pedCross.add(btnPedCross);
		
		//Zebrastreifen: Fußgänger-Distanz
		JPanel mainPanel_west_pedCross_distance = new JPanel();
		mainPanel_west.add(mainPanel_west_pedCross_distance);
		mainPanel_west_pedCross_distance.setLayout(new GridLayout(1, 0, 0, 0));
		
		JLabel lblpedCross_distance = new JLabel("");
		lblpedCross_distance.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_west_pedCross_distance.add(lblpedCross_distance);
		
		JTextArea pedCross_distance_textArea = new JTextArea();
		pedCross_distance_textArea.setText("m"); //TODO
		mainPanel_west_pedCross_distance.add(pedCross_distance_textArea);
		
		JButton btnPedCross_distance = new JButton("Fußgänger-Zeit-Distanz [10s -> 0s]");
		btnPedCross_distance.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.togglePedCrossDistance();
				controller.pedCross_distanz = !controller.pedCross_distanz;
			}
		});
		btnPedCross_distance.setBackground(Color.cyan);
		btnPedCross_distance.setOpaque(true);
		btnPedCross_distance.setBorderPainted(false);
		mainPanel_west_pedCross_distance.add(btnPedCross_distance);
		
		
		
		//Safe Stop
		JPanel mainPanel_west_safeStop = new JPanel();
		mainPanel_west.add(mainPanel_west_safeStop);
		mainPanel_west_safeStop.setLayout(new GridLayout(1, 0, 0, 0));
		
		JLabel lblSafeStop = new JLabel("");
		lblSafeStop.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_west_safeStop.add(lblSafeStop);
		
		JButton btnSafeStop = new JButton("Safe Stop EMERGENCY");
		btnSafeStop.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.toggleSafeStop();
			}
		});
		btnSafeStop.setBackground(Color.WHITE);
		btnSafeStop.setOpaque(true);
		btnSafeStop.setBorderPainted(false);
		mainPanel_west_safeStop.add(btnSafeStop);
		
		//MAIN PANEL EAST PANELS:
		//Wendemanöver rechts		
		JPanel mainPanel_east_TurnRight = new JPanel();
		mainPanel_east.add(mainPanel_east_TurnRight);
		mainPanel_east_TurnRight.setLayout(new GridLayout(1, 0, 0, 0));
		
		JButton btnTurnRight = new JButton("Wendemanöver auf Punkt rechts");
		btnTurnRight.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.toggleTurnRight();
			}
		});
		btnTurnRight.setBackground(Color.WHITE);
		btnTurnRight.setOpaque(true);
		btnTurnRight.setBorderPainted(false);
		mainPanel_east_TurnRight.add(btnTurnRight);
		
		JLabel lblTurnRight = new JLabel("");
		lblTurnRight.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_east_TurnRight.add(lblTurnRight);
		
		//Wendemanöver rechts: Wende geplant in (s):	
		JPanel mainPanel_east_TurnRightPlannedIn = new JPanel();
		mainPanel_east.add(mainPanel_east_TurnRightPlannedIn);
		mainPanel_east_TurnRightPlannedIn.setLayout(new GridLayout(1, 0, 0, 0));
		
		JButton btnTurnRightPlannedIn = new JButton("Wende rechts geplant in [15s -> 0s]");
		btnTurnRightPlannedIn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.toggleTurnRightPlannedIn();
				controller.Wende_rechts_geplant_in = !controller.Wende_rechts_geplant_in;
			}
		});
		btnTurnRightPlannedIn.setBackground(Color.cyan);
		btnTurnRightPlannedIn.setOpaque(true);
		btnTurnRightPlannedIn.setBorderPainted(false);
		mainPanel_east_TurnRightPlannedIn.add(btnTurnRightPlannedIn);
		
		JLabel lblTurnRightPlannedIn = new JLabel("");
		lblTurnRightPlannedIn.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_east_TurnRightPlannedIn.add(lblTurnRightPlannedIn);
		
		JTextArea turnRightPlannedIn_textArea = new JTextArea();
		turnRightPlannedIn_textArea.setText(controller.Wende_rechts_geplant_in + "s");
		mainPanel_east_TurnRightPlannedIn.add(turnRightPlannedIn_textArea);
		
		
		//Wendemanöver links		
		JPanel mainPanel_east_TurnLeft = new JPanel();
		mainPanel_east.add(mainPanel_east_TurnLeft);
		mainPanel_east_TurnLeft.setLayout(new GridLayout(1, 0, 0, 0));
		
		JButton btnTurnLeft = new JButton("Wendemanöver auf Punkt links");
		btnTurnLeft.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.toggleTurnLeft();
				controller.Wende_links_geplant_in = !controller.Wende_links_geplant_in;
			}
		});
		btnTurnLeft.setBackground(Color.WHITE);
		btnTurnLeft.setOpaque(true);
		btnTurnLeft.setBorderPainted(false);
		mainPanel_east_TurnLeft.add(btnTurnLeft);
		
		JLabel lblTurnLeft = new JLabel("");
		lblTurnLeft.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_east_TurnLeft.add(lblTurnLeft);
		
		//Wendemanöver links: Wende geplant in (s):	
		JPanel mainPanel_east_TurnLeftPlannedIn = new JPanel();
		mainPanel_east.add(mainPanel_east_TurnLeftPlannedIn);
		mainPanel_east_TurnLeftPlannedIn.setLayout(new GridLayout(1, 0, 0, 0));
		
		JButton btnTurnLeftPlannedIn = new JButton("Wende links geplant in [15s -> 0s]");
		btnTurnLeftPlannedIn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.toggleTurnLeftPlannedIn();
			}
		});
		btnTurnLeftPlannedIn.setBackground(Color.cyan);
		btnTurnLeftPlannedIn.setOpaque(true);
		btnTurnLeftPlannedIn.setBorderPainted(false);
		mainPanel_east_TurnLeftPlannedIn.add(btnTurnLeftPlannedIn);
		
		JLabel lblTurnLeftPlannedIn = new JLabel("");
		lblTurnLeftPlannedIn.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_east_TurnLeftPlannedIn.add(lblTurnLeftPlannedIn);
		
		JTextArea turnLeftPlannedIn_textArea = new JTextArea();
		turnLeftPlannedIn_textArea.setText(controller.Wende_links_geplant_in + "s");
		mainPanel_east_TurnLeftPlannedIn.add(turnLeftPlannedIn_textArea);
		
		
		//Wende beendet (links/rechts)
		JPanel mainPanel_east_TurningFinished = new JPanel();
		mainPanel_east.add(mainPanel_east_TurningFinished);
		mainPanel_east_TurningFinished.setLayout(new GridLayout(1, 0, 0, 0));
		
		JButton btnTurningFinished = new JButton("Wende beendet");
		btnTurningFinished.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.toggleTurningFinished();
				controller.wendeBeendet = !controller.wendeBeendet;
			}
		});
		btnTurningFinished.setBackground(Color.cyan);
		btnTurningFinished.setOpaque(true);
		btnTurningFinished.setBorderPainted(false);
		mainPanel_east_TurningFinished.add(btnTurningFinished);
		
		JLabel lblTurningFinished = new JLabel("");
		lblTurningFinished.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_east_TurningFinished.add(lblTurningFinished);
		
		
		//Einparken rechts
		JPanel mainPanel_east_ParkRight = new JPanel();
		mainPanel_east.add(mainPanel_east_ParkRight);
		mainPanel_east_ParkRight.setLayout(new GridLayout(1, 0, 0, 0));
		
		JButton btnParkRight = new JButton("Einparken rechts 90°");
		btnParkRight.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.toggleParkRight();
			}
		});
		btnParkRight.setOpaque(true);
		btnParkRight.setBorderPainted(false);
		btnParkRight.setBackground(Color.WHITE);
		mainPanel_east_ParkRight.add(btnParkRight);
		
		JLabel lblParkRight = new JLabel("");
		lblParkRight.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_east_ParkRight.add(lblParkRight);
		
		//Einparken links
		JPanel mainPanel_east_ParkLeft = new JPanel();
		mainPanel_east.add(mainPanel_east_ParkLeft);
		mainPanel_east_ParkLeft.setLayout(new GridLayout(1, 0, 0, 0));
		
		JButton btnParkLeft = new JButton("Einparken links 90°");
		btnParkLeft.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.toggleParkLeft();
			}
		});
		btnParkLeft.setOpaque(true);
		btnParkLeft.setBorderPainted(false);
		btnParkLeft.setBackground(Color.WHITE);
		mainPanel_east_ParkLeft.add(btnParkLeft);
		
		JLabel lblParkLeft = new JLabel("");
		lblParkLeft.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_east_ParkLeft.add(lblParkLeft);
		
		//Parken abbrechen	
		JPanel mainPanel_east_parkingAborted = new JPanel();
		mainPanel_east.add(mainPanel_east_parkingAborted);
		mainPanel_east_parkingAborted.setLayout(new GridLayout(1, 0, 0, 0));
		
		JButton btnParkingAborted = new JButton("Parken abbrechen");
		btnParkingAborted.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.toggleParkingAborted();
				controller.parkenAbbrechen = !controller.parkenAbbrechen;
			}
		});
		btnParkingAborted.setBackground(Color.cyan);
		btnParkingAborted.setOpaque(true);
		btnParkingAborted.setBorderPainted(false);
		mainPanel_east_parkingAborted.add(btnParkingAborted);
		
		JLabel lblParkingAborted = new JLabel("");
		lblParkingAborted.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_east_parkingAborted.add(lblParkingAborted);
		
		//Parken beendet	
		JPanel mainPanel_east_parkingFinished = new JPanel();
		mainPanel_east.add(mainPanel_east_parkingFinished);
		mainPanel_east_parkingFinished.setLayout(new GridLayout(1, 0, 0, 0));
		
		JButton btnParkingFinished = new JButton("Parken beendet");
		btnParkingFinished.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.toggleParkingFinished();
				controller.parkenBeendet = !controller.parkenBeendet;
			}
		});
		btnParkingFinished.setBackground(Color.cyan);
		btnParkingFinished.setOpaque(true);
		btnParkingFinished.setBorderPainted(false);
		mainPanel_east_parkingFinished.add(btnParkingFinished);
		
		JLabel lblParkingFinished = new JLabel("");
		lblParkingFinished.setHorizontalAlignment(SwingConstants.CENTER);
		mainPanel_east_parkingFinished.add(lblParkingFinished);
		
		
		//_____________
		
		contentPane.addComponentListener(new ComponentAdapter() {
			@Override
			public void componentResized(ComponentEvent e) {
				reloadImages();
			}
		});
		
		/**
		 * Setting up the DataSubscribers
		 */
		// Server
		controller.server.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				Server server = controller.server.getData();
				
				textField_IP.setText(server.address);
				textField_Port.setText("" + server.port);
			}
		});
		
		// Connection Status
		controller.connection_status.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				ConnectionStatus cs = controller.connection_status.getData();
				
				lblConnectionstatus.setText("<html>ConnectionStatus: "
						+ (cs == ConnectionStatus.CONNECTED ? "<p style=\"color:green;\">CONNECTED</p>" : "")
						+ (cs == ConnectionStatus.DISCONNECTED ? "<p style=\"color:red;\">DISCONNECTED</p>" : "")
						+ (cs == ConnectionStatus.CONNECTING ? "<p style=\"color:blue;\">CONNECTING</p>" : "")
						+ "</html>");
				
				switch(cs) {
				case CONNECTED: 
				case CONNECTING: btnReconnect.setVisible(false); break;
				case DISCONNECTED: btnReconnect.setVisible(true); break;
				}
				
				switch(cs) {
				case CONNECTED: btnDisconnect.setVisible(true); break;
				case CONNECTING:
				case DISCONNECTED: btnDisconnect.setVisible(false); break;
				}
				
				switch(cs) {
				case CONNECTED: 
				case CONNECTING: btnEditIP.setVisible(false); btnEditPort.setVisible(false); break;
				case DISCONNECTED: btnEditIP.setVisible(true); btnEditPort.setVisible(true); break;
				}
			}
		});
		
		//Updates the "pedCross"-TextField in the GUI -> takes value from server
		controller.GUI_time_distance_pedestrian.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				float GUI_time_distance_pedestrian = controller.GUI_time_distance_pedestrian.getData();
				boolean pedCross_distanz = controller.pedCross_distanz;
				
				pedCross_distance_textArea.setText(String.valueOf(GUI_time_distance_pedestrian) + "s"); 
				
				if(pedCross_distanz) {
					btnPedCross_distance.setBackground(Color.ORANGE);
				} else {
					btnPedCross_distance.setBackground(Color.cyan);
				}
				
			}
		});
		
		
		//Updates the "allowPass"-TextField in the GUI -> takes value from server
		controller.GUI_distance_passing_vehicle.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				float GUI_distance_passing_vehicle = controller.GUI_distance_passing_vehicle.getData();
				boolean fahrzeug_distanz = controller.fahrzeug_distanz;
				
				allowPass_distance_textArea.setText(String.valueOf(GUI_distance_passing_vehicle) + "m"); 
				
				if(fahrzeug_distanz) {
					btnAllowPass_distance.setBackground(Color.ORANGE);
				} else {
					btnAllowPass_distance.setBackground(Color.cyan);
				}
					
			}
		});
		
		//Updates the "TurningLeft"-TextField in the GUI -> takes value from server
		controller.GUI_turning_left_dynamic_module_planned_t.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				float GUI_turning_left_dynamic_module_planned_t = controller.GUI_turning_left_dynamic_module_planned_t.getData();
				boolean Wende_links_geplant_in = controller.Wende_links_geplant_in;
				
				turnLeftPlannedIn_textArea.setText(String.valueOf(GUI_turning_left_dynamic_module_planned_t) + "s"); 
					
				if(Wende_links_geplant_in) {
					btnTurnLeftPlannedIn.setBackground(Color.ORANGE);
				} else {
					btnTurnLeftPlannedIn.setBackground(Color.cyan);
				}
					
				
			}
		});
		
		//Updates the "TurningRight"-TextField in the GUI -> takes value from server
		controller.GUI_turning_right_dynamic_module_planned_t.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				float GUI_turning_right_dynamic_module_planned_t = controller.GUI_turning_right_dynamic_module_planned_t.getData();
				boolean Wende_rechts_geplant_in = controller.Wende_rechts_geplant_in;
				
				turnRightPlannedIn_textArea.setText(String.valueOf(GUI_turning_right_dynamic_module_planned_t) + "s"); 
					
				if(Wende_rechts_geplant_in) {
					btnTurnRightPlannedIn.setBackground(Color.ORANGE);
				} else {
					btnTurnRightPlannedIn.setBackground(Color.cyan);
				}
				
			}
		});
		
		//Turning Finished Button Color
		controller.turning_finished_status.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				boolean turning_finished_status = controller.turning_finished_status.getData();
				
				if (turning_finished_status) {
					btnTurningFinished.setBackground(Color.ORANGE);
					
				} else {
					btnTurningFinished.setBackground(Color.cyan);
				}
			}
		});
		
		//Parking Aborted Button Colour
		controller.parking_aborted_status.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				boolean parking_aborted_status = controller.parking_aborted_status.getData();
				
				if (parking_aborted_status) {
					btnParkingAborted.setBackground(Color.ORANGE);
					
				} else {
					btnParkingAborted.setBackground(Color.cyan);
				}
			}
		});
		
		//Parking Finished
		controller.parking_finished_status.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				boolean parking_finished_status = controller.parking_finished_status.getData();
				
				if (parking_finished_status) {
					btnParkingFinished.setBackground(Color.ORANGE);
					
				} else {
					btnParkingFinished.setBackground(Color.cyan);
				}
			}
		});
		
		
		
		//Loading Status
		controller.loading_status.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				boolean loading_status = controller.loading_status.getData();
				
				if (loading_status) {
					btnLoading.setBackground(Color.ORANGE);
					lblLoading.setIcon(loadingOn);
				} else {
					btnLoading.setBackground(Color.WHITE);
					lblLoading.setIcon(loadingOff);
				}
			}
		});
	
		//Start Right Status
		controller.start_right_status.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				boolean start_right_status = controller.start_right_status.getData();
				
				if (start_right_status) {
					btnLParkRight.setBackground(Color.ORANGE);
					lblLParkRight.setIcon(parkRightOn);
				} else {
					btnLParkRight.setBackground(Color.WHITE);
					lblLParkRight.setIcon(parkRightOff);
				}
			}
		});
		
		//Start Left Status
		controller.start_left_status.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				boolean start_left_status = controller.start_left_status.getData();
				
				if (start_left_status) {
					btnLParkLeft.setBackground(Color.ORANGE);
					lblLParkLeft.setIcon(parkLeftOn);
				} else {
					btnLParkLeft.setBackground(Color.WHITE);
					lblLParkLeft.setIcon(parkLeftOff);
				}
			}
		});
		
		//Allow Pass (Engstelle)
		controller.allow_pass_status.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				boolean allow_pass_status = controller.allow_pass_status.getData();
				
				if (allow_pass_status) {
					btnAllowPass.setBackground(Color.ORANGE);
					lblAllowPass.setIcon(allowPassOn);
				} else {
					btnAllowPass.setBackground(Color.WHITE);
					lblAllowPass.setIcon(allowPassOff);
				}
			}
		});
		
		//Ped Cross
		controller.ped_cross_status.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				boolean ped_cross_status = controller.ped_cross_status.getData();
				
				if (ped_cross_status) {
					btnPedCross.setBackground(Color.ORANGE);
					lblPedCross.setIcon(pedCrossOn);
				} else {
					btnPedCross.setBackground(Color.WHITE);
					lblPedCross.setIcon(pedCrossOff);
				}
			}
		});
		
		//Safe Stop
		controller.safe_stop_status.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				boolean safe_stop_status = controller.safe_stop_status.getData();
				
				if (safe_stop_status) {
					btnSafeStop.setBackground(Color.ORANGE);
					lblSafeStop.setIcon(safeStopOn);
				} else {
					btnSafeStop.setBackground(Color.WHITE);
					lblSafeStop.setIcon(safeStopOff);
				}
			}
		});
		
		//Turning Right
		controller.turning_right_status.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				boolean turning_right_status = controller.turning_right_status.getData();
				
				if (turning_right_status) {
					btnTurnRight.setBackground(Color.ORANGE);
					lblTurnRight.setIcon(turningRightOn);
				} else {
					btnTurnRight.setBackground(Color.WHITE);
					lblTurnRight.setIcon(turningRightOff);
				}
			}
		});
		
		//Turning Left
		controller.turning_left_status.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				boolean turning_left_status = controller.turning_left_status.getData();
				
				if (turning_left_status) {
					btnTurnLeft.setBackground(Color.ORANGE);
					lblTurnLeft.setIcon(turningLeftOn);
				} else {
					btnTurnLeft.setBackground(Color.WHITE);
					lblTurnLeft.setIcon(turningLeftOff);
				}
			}
		});
		
		//Stop right
		controller.stop_right_status.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				boolean stop_right_status = controller.stop_right_status.getData();
				
				if (stop_right_status) {
					btnParkRight.setBackground(Color.ORANGE);
					lblParkRight.setIcon(parkRightOn);
				} else {
					btnParkRight.setBackground(Color.WHITE);
					lblParkRight.setIcon(parkRightOff);
				}
			}
		});
		
		//Stop left
		controller.stop_left_status.subscribe(new DataSubscriber() {

			@Override
			public void notifySub() {
				boolean stop_left_status = controller.stop_left_status.getData();
				
				if (stop_left_status) {
					btnParkLeft.setBackground(Color.ORANGE);
					lblParkLeft.setIcon(parkLeftOn);
				} else {
					btnParkLeft.setBackground(Color.WHITE);
					lblParkLeft.setIcon(parkLeftOff);
				}
			}
		});
		
	} //end of constructor
	
	//class method
	private void reloadImages() {
		if (contentPane == null) {
			return;
		}
		
		int width = Math.min(contentPane.getWidth(), contentPane.getHeight()) / 10;
		
		loadingOn = new ImageIcon(loadingOn_image.getScaledInstance(width, width, Image.SCALE_FAST));
		loadingOff = new ImageIcon(loadingOff_image.getScaledInstance(width, width, Image.SCALE_FAST));
		
		safeStopOn = new ImageIcon(safeStopOn_image.getScaledInstance(width, width, Image.SCALE_FAST));
		safeStopOff = new ImageIcon(safeStopOff_image.getScaledInstance(width, width, Image.SCALE_FAST));
		
		allowPassOn = new ImageIcon(allowPassOn_image.getScaledInstance(width, width, Image.SCALE_FAST));
		allowPassOff = new ImageIcon(allowPassOff_image.getScaledInstance(width, width, Image.SCALE_FAST));
		
		parkLeftOn = new ImageIcon(parkLeftOn_image.getScaledInstance(width, width, Image.SCALE_FAST));
		parkLeftOff = new ImageIcon(parkLeftOff_image.getScaledInstance(width, width, Image.SCALE_FAST));
		
		parkRightOn = new ImageIcon(parkRightOn_image.getScaledInstance(width, width, Image.SCALE_FAST));
		parkRightOff = new ImageIcon(parkRightOff_image.getScaledInstance(width, width, Image.SCALE_FAST));
		
		pedCrossOn = new ImageIcon(pedCrossOn_image.getScaledInstance(width, width, Image.SCALE_FAST));
		pedCrossOff = new ImageIcon(pedCrossOff_image.getScaledInstance(width, width, Image.SCALE_FAST));
		
		turningLeftOn = new ImageIcon(turningLeftOn_image.getScaledInstance(width, width, Image.SCALE_FAST));
		turningLeftOff = new ImageIcon(turningLeftOff_image.getScaledInstance(width, width, Image.SCALE_FAST));
		
		turningRightOn = new ImageIcon(turningRightOn_image.getScaledInstance(width, width, Image.SCALE_FAST));
		turningRightOff = new ImageIcon(turningRightOff_image.getScaledInstance(width, width, Image.SCALE_FAST));		
		
		controller.loading_status.notifySubs();
		controller.start_right_status.notifySubs();
		controller.start_left_status.notifySubs();
		controller.allow_pass_status.notifySubs();
		controller.ped_cross_status.notifySubs();
		controller.safe_stop_status.notifySubs();
		controller.turning_right_status.notifySubs();
		controller.turning_left_status.notifySubs();
		controller.stop_right_status.notifySubs();
		controller.stop_left_status.notifySubs();
			
		}
	}