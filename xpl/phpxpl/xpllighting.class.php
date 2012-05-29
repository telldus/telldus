<?php

require_once 'xplinstance.class.php';

class xPLLighting extends XPLInstance {

	public function __construct($ip) {
		parent::__construct('telldus', 'core', $ip);
	}

	protected function attatchedToNetwork() {
		$message = new xPLMessage( xPLMessage::xpltrig );
		$message->setMessageSchemeIdentifier( 'lighting.gateway' );
		$message->setTarget('*');
		$message->setSource( $this->_thisDevice->deviceName() );
		$message->addBodyItem( 'report', 'gateway-ready' );
		$this->sendMessage( $message );
	}

	protected function handleMessage( $message ) {
		if ($message->messageSchemeIdentifier() == 'lighting.request') {
			return $this->lightingRequest($message);
		}
		return false;
	}

	private function lightingRequest( $message ) {
		switch( $message->bodyItem('request') ) {
			case 'gateinfo':
				return $this->gateInfo(  );
			case 'netlist':
				return $this->netList(  );
			case 'netinfo':
				return $this->netInfo( $message );
			case 'devlist':
				return $this->devList( $message );
			case 'devinfo':
				return $this->devInfo( $message );
		}
		return false;
	}

	private function gateInfo( ) {
		$message = new xPLMessage( xPLMessage::xplstat );
		$message->setMessageSchemeIdentifier( 'lighting.gateinfo' );
		$message->setTarget('*');
		$message->setSource( $this->_thisDevice->deviceName() );
		$message->addBodyItem( 'status', 'ok' );
		$message->addBodyItem( 'protocol', 'TELLDUS' );
		$message->addBodyItem( 'description', 'xPL to Telldus TellStick gateway' );
		$message->addBodyItem( 'version', '1.0' );
		$message->addBodyItem( 'author', 'Telldus Technologies AB' );
		$message->addBodyItem( 'info-url', 'http://www.telldus.se' );
		$message->addBodyItem( 'net-count', '1' );
		$message->addBodyItem( 'preferred-net', '1' );
		$message->addBodyItem( 'scenes-ok', 'false' );
		$message->addBodyItem( 'channels-ok', 'false' );
		$message->addBodyItem( 'fade-rate-ok', 'false' );
		$this->sendMessage( $message );
		return true;
	}

	private function netList( ) {
		$message = new xPLMessage( xPLMessage::xplstat );
		$message->setMessageSchemeIdentifier( 'lighting.netlist' );
		$message->setTarget('*');
		$message->setSource( $this->_thisDevice->deviceName() );
		$message->addBodyItem( 'status', 'ok' );
		$message->addBodyItem( 'network', '1' );
		$this->sendMessage( $message );
		return true;
	}

	private function netInfo( $message ) {
		$msg = new xPLMessage( xPLMessage::xplstat );
		$msg->setMessageSchemeIdentifier( 'lighting.netinfo' );
		$msg->setTarget('*');
		$msg->setSource( $this->_thisDevice->deviceName() );
// 		print_r($message);
		$msg->addBodyItem( 'network', $message->bodyItem('network') );
		if ($msg->bodyItem('network') == '1') {
			$msg->addBodyItem( 'status', 'ok' );
			$msg->addBodyItem( 'name', exec('hostname') );
			$msg->addBodyItem( 'device-count', tdGetNumberOfDevices() );
			$msg->addBodyItem( 'scene-count', 0 );
		} else {
			$msg->addBodyItem( 'status', 'not-found' );
		}
		$this->sendMessage( $msg );
		return true;
	}

	private function devList( $message ) {
		$msg = new xPLMessage( xPLMessage::xplstat );
		$msg->setMessageSchemeIdentifier( 'lighting.devlist' );
		$msg->setTarget('*');
		$msg->setSource( $this->_thisDevice->deviceName() );
		$msg->addBodyItem( 'network', $message->bodyItem('network') );
		if ($msg->bodyItem('network') == '1') {
			$msg->addBodyItem( 'status', 'ok' );
			$count = tdGetNumberOfDevices();
			$deviceList = array();
			for( $i = 0; $i < $count; ++$i ) {
				$deviceList[] = tdGetDeviceId($i);
			}
			$msg->addBodyItem( 'device-count', $count );
			$msg->addBodyItem( 'device', implode(',', $deviceList) );
		} else {
			$msg->addBodyItem( 'status', 'not-found' );
		}
		$this->sendMessage( $msg );
		return true;
	}

	private function devInfo( $message ) {
		$found = false;
		$count = tdGetNumberOfDevices();
		$deviceId = $message->bodyItem('device');
		for( $i = 0; $i < $count; ++$i ) {
			if (tdGetDeviceId($i) == $deviceId) {
				$found = true;
				break;
			}
		}

		$msg = new xPLMessage( xPLMessage::xplstat );
		$msg->setMessageSchemeIdentifier( 'lighting.devinfo' );
		$msg->addBodyItem( 'network', $message->bodyItem('network') );
		$msg->addBodyItem( 'device', $deviceId );
		$msg->setTarget('*');
		$msg->setSource( $this->_thisDevice->deviceName() );

		if ($msg->bodyItem('network') == '1' && $found) {
			$methods = tdMethods($deviceId, TELLDUS_TURNON | TELLDUS_TURNOFF | TELLDUS_DIM );
			$msg->addBodyItem( 'status', 'ok' );
			$msg->addBodyItem( 'name', tdGetName($deviceId) );
			$msg->addBodyItem( 'report-on-manual', 'false' );
			$msg->addBodyItem( 'channel-count', '1');
			$msg->addBodyItem( 'primary-channel', '1');
			$msg->addBodyItem( 'channel', sprintf('1,%s,0,0', ($methods & TELLDUS_DIM ? 'true' : 'false')) );
			$msg->addBodyItem( 'scene-count', '0');
		} else {
			$msg->addBodyItem( 'status', 'not-found' );
		}
		$this->sendMessage( $msg );
		return true;
	}

	private function scnList( $message ) {
		$msg = new xPLMessage( xPLMessage::xplstat );
		$msg->setMessageSchemeIdentifier( 'lighting.scnlist' );
		$msg->setTarget('*');
		$msg->setSource( $this->_thisDevice->deviceName() );
		$msg->addBodyItem( 'network', $message->bodyItem('network') );
		$msg->addBodyItem( 'status', 'not-found' );
		$this->sendMessage( $msg );
		return true;
	}

	private function scnInfo( $message ) {
		$msg = new xPLMessage( xPLMessage::xplstat );
		$msg->setMessageSchemeIdentifier( 'lighting.scninfo' );
		$msg->addBodyItem( 'network', $message->bodyItem('network') );
		$msg->addBodyItem( 'scene', $message->bodyItem('scene') );
		$msg->setTarget('*');
		$msg->setSource( $this->_thisDevice->deviceName() );
		$msg->addBodyItem( 'status', 'not-found' );
		$this->sendMessage( $msg );
		return true;
	}

}
