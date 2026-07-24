package JantarFilosofos;

import java.util.concurrent.Semaphore;

public class Jantar {
	private static final int GARFOS = 5;
	private static final int FILOSOFOS = 5;
	
	public static void main(String[] args) {
		Semaphore garfos = new Semaphore(GARFOS);
		
		for(int i=1; i<=FILOSOFOS; i++) {
			Runnable comer = new Comer("filosofo " + i, garfos);
			new Thread(comer).start();
		}
	}
}
