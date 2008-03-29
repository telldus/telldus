import java.io.*;

class test
{
	tellstick TS = new tellstick();

	public void run() {
		try {
			BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

			int nbrDevices = TS.devGetNumberOfDevices();
			for (int i = 0; i < nbrDevices; i++) {
				int id = TS.devGetDeviceId(i);
				String deviceName = TS.devGetName(id);
				System.out.println(id + "\t" + deviceName);
			}

			System.out.print("Enter a device: ");
			int device = Integer.parseInt(in.readLine());
			process(device);

		} catch (IOException e) {
		}
	}

	private void process(int device) {
		try {
			BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
			int methods = TS.devMethods(device);
			if ((methods & TS.TELLSTICK_TURNON) > 0) {
				System.out.println(TS.TELLSTICK_TURNON + "\tTurn on");
			}
			if ((methods & TS.TELLSTICK_TURNOFF) > 0) {
				System.out.println(TS.TELLSTICK_TURNOFF + "\tTurn off");
			}
			if ((methods & TS.TELLSTICK_BELL) > 0) {
				System.out.println(TS.TELLSTICK_BELL + "\tBell");
			}
			if ((methods & TS.TELLSTICK_DIM) > 0) {
				System.out.println(TS.TELLSTICK_DIM + "\tDim");
			}
			System.out.println("What do you want to do?");
			int action = Integer.parseInt(in.readLine());
			if ( (action & methods) == 0 ) {
				System.out.println("The device doesn't support this method");
				return;
			}

			if ((action & TS.TELLSTICK_TURNON) > 0) {
				turnOn(device);
			} else if ((action & TS.TELLSTICK_TURNOFF) > 0) {
				turnOff(device);
			} else if ((action & TS.TELLSTICK_BELL) > 0) {
				bell(device);
			} else if ((action & TS.TELLSTICK_DIM) > 0) {
				dim(device);
			}

		} catch (IOException e) {
		}
	}

	private void turnOn(int device) {
		System.out.println("Turning on " + TS.devGetName(device));
		TS.devTurnOn(device);
	}

	private void turnOff(int device) {
		System.out.println("Turning off " + TS.devGetName(device));
		TS.devTurnOff(device);
	}

	private void bell(int device) {
		System.out.println("Sending bell to " + TS.devGetName(device));
		TS.devBell(device);
	}

	private void dim(int device) {
		try {
			BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
			System.out.print("Select level (0-255): ");
			int level = Integer.parseInt(in.readLine());
			if (level >= 256 || level < 0) {
				System.out.println("Out of range");
			}
			System.out.println("Dim " + TS.devGetName(device));
			TS.devDim(device, level);
			} catch (IOException e) {
		}
	}

	public static void main(String[] args)
	{
		test t = new test();
		t.run();
	}

}