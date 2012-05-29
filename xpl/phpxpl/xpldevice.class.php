<?php
define('DEFAULT_HBEAT_INTERVAL', 5);

class xPLDevice {
	
	private $_heartBeatInterval = DEFAULT_HBEAT_INTERVAL;
	private $_port = 0;
	private $_vendor = '';
	private $_device = '';
	private $_instance = '';
	private $_address = '';
	private $_lastHeartBeat = 0;
	
	public function __construct( $vendor, $device = null ) {
		if (is_null($device)) {
			$dash = strpos($vendor, '-');
			$dot = strpos($vendor, '.');
			$this->_vendor = substr($vendor, 0, $dash);
			$this->_device = substr($vendor, $dash+1, $dot-$dash-1);
			$this->_instance = substr($vendor, $dot+1);
		} else {
			$this->_vendor = $vendor;
			$this->_device = $device;
			
			//Generate an instance
			$randomNumber = mt_rand(1, 9999); //Generate ranom number between 1-9999
			$this->_instance = sprintf('default%s', $randomNumber);
		}
	}

	public function address() {
		return $this->_address;
	}

	public function heartBeatInterval() {
		return $this->_heartBeatInterval;
	}

	public function deviceName() {
		return sprintf('%s-%s.%s', $this->_vendor, $this->_device, $this->_instance);
	}

	public function lastHeartBeat() {
		return $this->_lastHeartBeat;
	}

	public function port() {
		return $this->_port;
	}

	public function setAddress( $address ) {
		$this->_address = $address;
	}

	public function setHeartBeatInterval( $minutes ) {
		$this->_heartBeatInterval = $minutes;
	}

	public function setPort( $p ) {
		$this->_port = $p;
	}

	public function setLastHeartBeat( $timestamp ) {
		$this->_lastHeartBeat = $timestamp;
	}
}
