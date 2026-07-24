package JantarFilosofos;

import java.util.concurrent.Semaphore;

public class Comer implements Runnable {
	private String filosofo;
	private Semaphore garfo;
	
	public Comer(String filosofo, Semaphore garfo) {
		this.filosofo = filosofo;
		this.garfo = garfo;
	}
	@Override
	public void run() {
		try {
			System.out.println(filosofo + " Esta tentando comer");
			garfo.acquire(2);
			System.out.println(filosofo + " Esta comendo");
			Thread.sleep((int)(Math.random() * 4000) + 1000);
			System.out.println(filosofo + " Terminou de comer");
			garfo.release(2);
		}catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}
