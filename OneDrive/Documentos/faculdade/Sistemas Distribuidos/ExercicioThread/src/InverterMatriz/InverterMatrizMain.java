package InverterMatriz;

import java.util.concurrent.CyclicBarrier;

public class InverterMatrizMain {
	private static int[][] matriz = { //quando usar private e public para threads ?
			{1, 2, 3, 4},
			{4, 5, 6, 7},
			{7, 8, 9, 0},
			{11, 12, 13, 14}
	};
	
	public static void main(String[] args) {
		CyclicBarrier barreira = new CyclicBarrier(matriz.length, new Imprimir());
		for(int i=0; i<matriz.length;i++){
			Thread linha = new Thread(new InverterThread(matriz[i], barreira));
			linha.start();
		}
	}
	
	public static class Imprimir implements Runnable{
		@Override
		public void run() {
			new EscreverMatriz(matriz).run();
		}
	}
}
