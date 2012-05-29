<?php

require_once 'xplmessage.class.php';
require_once 'xpldevice.class.php';

define( XPL_PORT, 3865 );

class XPLInstance {
	protected $_thisDevice = null;
	protected $_devices = array();
	
	private $_socket;
	private $_isAttached = false;
	private $_ip;
	
	public function __construct($vendor, $device, $ip) {
		$this->_ip = $ip;
		$this->_thisDevice = new XPLDevice($vendor, $device);
		
		$this->init();
	}
	
	public function attatched() {
		return $this->_isAttatched;
	}

	public function detach() {
		$message = new xPLMessage( xPLMessage::xplstat );
		$message->setMessageSchemeIdentifier( 'hbeat.end' );
		$message->setTarget('*');
		$message->setSource( $this->_thisDevice->deviceName() );
		$this->sendMessage( $message );
		socket_close($this->_socket);
		$this->_socket = null;
	}
	
	public function doEvents() {
		$string = socket_read( $this->_socket, 1500);
		if ($string != '') {
			$message = XPLMessage::createMessageFromString($string);
			if ($message) {
				$this->processMessage( $message );
			}
			return false;
		}
		$this->poll();
		return true;
	}
	
	public function sendMessage( $message, $device = null ) {
		//echo "Sending heartbeat\n";
		$msg = (string)$message;
		socket_sendto( $this->_socket, $msg, strlen($msg), 0, '255.255.255.255', XPL_PORT );
	}

	protected function attatchedToNetwork() {
	}
	
	protected function handleMessage( $message ) {
		return false;
	}
	
	
	private function bindToPort() {
		$port = 49152;
		//$port = XPL_PORT;
		$error = 98;
		while ($error == 98 && $port < 65535) {
			if (@socket_bind($this->_socket, $this->_ip, $port) === false) {
				++$port;
			} else {
				echo "Bind succeded as client on: " . $port . "\n";
				return $port;
			}
		}
		return 0;
	}

	private function init() {
		if (($this->_socket = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP)) === false) {
			echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
			return;
		}
		
		socket_set_option( $this->_socket, SOL_SOCKET, SO_BROADCAST, 1);
		socket_set_nonblock( $this->_socket );

		$this->_thisDevice->setPort( $this->bindToPort() );
		if ($this->_thisDevice->port() == 0) {
			echo "socket_bind() failed: reason: " . socket_strerror(socket_last_error($this->_socket)) . "\n";
			return;
		}

		printf("xPL server started (bound to port %s)\n", $this->_thisDevice->port());
		$this->sendHeartbeat();
	}
		
	private function poll() {
		if (!$this->attatched()) {
			$this->sendHeartbeat();
			return;
		}
		
		if (time()-$this->_thisDevice->lastHeartBeat() >= $this->_thisDevice->heartBeatInterval() * 60) {
			$this->sendHeartbeat();
		}
		//Loop all devices to see if they have timed out
		foreach( $this->_devices as $key => $device ) {
			if ( time()-$device->lastHeartBeat() >= $device->heartBeatInterval() * 60 * 2) {
				printf("Device removed (timeout): %s\n", $device->deviceName());

				//This is not a prefeered way of removing the item from an array
				//Since we are iterating the loop will miss to check the new item.
				//Here, it will be checked next time poll is called
				unset($this->_devices[$key]);
				continue;
			}
		}
	}
	
	private function processHeartBeat( $message ) {
		if ($message->messageSchemeIdentifier() == 'hbeat.request') {
			$this->sendHeartbeat();
			return true;
		}
		foreach( $this->_devices as $key => $device ) {
			if ( $device->deviceName() == $message->source() ) {
				if (substr($message->messageSchemeIdentifier(), -3) == 'end') {
					echo "Device removed: " . $message->source() . "\n";
					unset($this->_devices[$key]);
					return true;
				} else {
					$device->setLastHeartBeat( time() );
					$device->setHeartBeatInterval( $message->bodyItem( 'interval' ) );
				}
				return true;
			}
		}
		if ( (substr($message->messageSchemeIdentifier(), -3) != 'app') && (substr($message->messageSchemeIdentifier(), -5) != 'basic') ) { //Not a heartbeat
			return false;
		}
		
		printf( "New device: %s\n", $message->source() );
		$device =  new xPLDevice( $message->source() );
		$device->setHeartBeatInterval( $message->bodyItem('interval') );
		$device->setLastHeartBeat( time() );
		if ($message->messageSchemeIdentifier() == 'hbeat.app') {
			$device->setAddress( $message->bodyItem('remote-ip'));
			$device->setPort( $message->bodyItem('port') );
		}
		$this->_devices[] = $device;
		return true;
	}
	
	private function processMessage( $message ) {
		if (!$this->attatched() && $message->messageSchemeIdentifier() == 'hbeat.app' && $message->source() == $this->_thisDevice->deviceName()) {
			//Our own echo
			$this->setAttatched( true );
			return;
		}
		if ($message->source() == $this->_thisDevice->deviceName()) {
			//Ignore messages from ourselves
			return;
		}
		if ( (substr($message->messageSchemeIdentifier(), 0, 5) == 'hbeat') || (substr($message->messageSchemeIdentifier(), 0, 6) == 'config') ) {
			if ($this->processHeartBeat( $message )) {
				return;
			}
		}
		if ($message->target() != $this->_thisDevice->deviceName() && $message->target() != '*') {
			//Message not for us
			return;
		}
		if (!$this->handleMessage($message)) {
			printf("Got message:\n%s", $message);
		}
	}
	
	
	private function sendHeartbeat() {
// 		echo "Sending heartbeat\n";
		$message = new xPLMessage( xPLMessage::xplstat );
		$message->setMessageSchemeIdentifier( 'hbeat.app' );
		$message->setTarget('*');
		$message->setSource( $this->_thisDevice->deviceName() );
		$message->addBodyItem( 'interval', $this->_thisDevice->heartBeatInterval() );
		$message->addBodyItem( 'port', $this->_thisDevice->port() );
		$message->addBodyItem( 'remote-ip', $this->_ip );
		$this->sendMessage( $message );
		$this->_thisDevice->setLastHeartBeat( time() );	
	}
	
	private function setAttatched( $attatched ) {
		$this->_isAttatched = $attatched;
		if ($this->_isAttatched) {
			echo "Attatched to xPL-network\n";
//			timer->setInterval( 60000 ); //Once a minute

			$message = new xPLMessage( xPLMessage::xplcmnd );
			$message->setTarget( "*" );
			$message->setSource( $this->_thisDevice->deviceName() );
			$message->setMessageSchemeIdentifier( 'hbeat.request' );
			$message->addBodyItem('command', 'request');
			$this->sendMessage( $message );

			$this->attatchedToNetwork();
		}
	}
		
}
