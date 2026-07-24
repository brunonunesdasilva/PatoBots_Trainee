package ProdutoEscalar;
import java.util.concurrent.CyclicBarrier;


public class Escalar {
	private static final int Num_threads = 3;
	public static final int[] resultados = new int[Num_threads];
	
	public static void main(String[] args) {
		int[] A = {1, 2, 3};
		int[] B = {1, 5, 7};
		
		CyclicBarrier barreira = new CyclicBarrier(Num_threads, new ProdutoE());
		
		for (int i=0; i < Num_threads; i++) {
			Thread pe = new Thread(new ProdutoEscalarThread(A[i], B[i], barreira, i));
			new Thread(pe).start();
		}
	}
	
	public static class ProdutoE implements Runnable{
		@Override
		public void run() {
			int somaTotal = 0;
			for(int valor: resultados) {
				somaTotal += valor;
			}
			System.out.println("o produto escalar é " + somaTotal);
		}
	}
}
