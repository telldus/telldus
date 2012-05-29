<?php

class xPLMessage {

	private $_hop = 1;
	private $_identifier = null;
	private $_body = array();
	private $_msi = '';
	private $_source = '';
	private $_target = '';
	
	const xplcmnd = 1;
	const xplstat = 2;
	const xpltrig = 3;
	
	public function __construct($identifier) {
		$this->_identifier = $identifier;
	}

	public function addBodyItem( $key, $value ) {
		$this->_body[$key] = $value;
	}

	public function addHeadItem( $key, $value ) {
		if ($key == 'hop') {
			$hop = (int)$value;
		} else if ($key == 'source') {
			$this->setSource($value);
		} else if ($key == 'target') {
			$this->setTarget($value);
		}
	}

	public function bodyItem( $key ) {
		if (array_key_exists($key, $this->_body)) {
			return $this->_body[$key];
		}
		return '';
	}

	public function messageSchemeIdentifier() {
		return $this->_msi;
	}

	public function source() {
		return $this->_source;
	}

	public function target() {
		return $this->_target;
	}

	public function setMessageSchemeIdentifier( $messageSchemeIdentifier ) {
		$this->_msi = $messageSchemeIdentifier;
	}

	public function setSource( $value ) {
		$this->_source = $value;
	}

	public function setTarget( $value ) {
		$this->_target = $value;
	}

	public function __tostring() {
		$message = '';

		switch( $this->_identifier ) {
			case self::xplcmnd:
				$message .= "xpl-cmnd\n";
				break;
			case self::xplstat:
				$message .= "xpl-stat\n";
				break;
			case self::xpltrig:
				$message .= "xpl-trig\n";
				break;
			default:
				return "";
		}

		$message .= "{\n";
		$message .= sprintf("hop=%s\n", $this->_hop);
		$message .= sprintf("source=%s\n", $this->_source);
		$message .= sprintf("target=%s\n", $this->_target);
		$message .= "}\n";
		$message .= $this->_msi . "\n";
		$message .= "{\n";
		foreach( $this->_body as $key => $value ) {
			$message .= sprintf("%s=%s\n", $key, $value);
		}
		$message .= "}\n";

		return $message;
	}

	public static function createMessageFromString( $message ) {
		$lines = explode("\n", $message);
		$row = 0;

		$i = 0;

		if ($lines[$row] == 'xpl-cmnd') {
			$i = self::xplcmnd;
		} else if ($lines[$row] == 'xpl-stat') {
			$i = self::xplstat;
		} else if ($lines[$row] == 'xpl-trig') {
			$i = self::xpltrig;
		} else {
			return 0;
		}
		++$row;

		if ($lines[$row] != '{') {
			return 0;
		}
		++$row;

		$msg = new xPLMessage($i);
		for(; $row < count($lines) && $lines[$row] != '}'; ++$row) {
			list($name, $value) = explode( '=', $lines[$row] );
			$msg->addHeadItem( $name, $value );
		}
		if ($row >= count($lines)) {
			return 0;
		}
		++$row;

		$msg->setMessageSchemeIdentifier( $lines[$row] );
		++$row;

		if ($lines[$row] != '{') {
			return 0;
		}
		++$row;

		for(; $row < count($lines) && $lines[$row] != '}'; ++$row) {
			list($name, $value) = explode( '=', $lines[$row] );
			$msg->addBodyItem( $name, $value );
		}

		return $msg;
	}
}
