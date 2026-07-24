package InverterMatriz;

import java.util.concurrent.CyclicBarrier;

public class InverterThread implements Runnable{
	private int[] linha;
	private CyclicBarrier barreira;
	
	public InverterThread(int[] linha, CyclicBarrier barreira) {
		this.linha = linha;
		this.barreira = barreira;
	}
	@Override
	public void run() {
		try {
			int fim = linha.length -1;
			for(int i = 0; i<(linha.length/2); i++) {
				int inicio = linha[i];
				linha[i] = linha[fim];
				linha[fim] = inicio;
				fim--;
			}
			barreira.await();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
