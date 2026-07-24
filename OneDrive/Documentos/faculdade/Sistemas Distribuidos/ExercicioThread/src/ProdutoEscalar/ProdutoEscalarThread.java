package ProdutoEscalar;
import java.util.concurrent.CyclicBarrier;

public class ProdutoEscalarThread implements Runnable {
	private int a, b, indice;
	private CyclicBarrier barreira;
	
	public ProdutoEscalarThread(int a, int b, CyclicBarrier barreira, int indice) {
		this.a = a;
		this.b = b;
		this.barreira = barreira;
		this.indice = indice;
		
	}
	@Override
	public void run() {
		try {
			Escalar.resultados[indice] = a * b;
			barreira.await();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
