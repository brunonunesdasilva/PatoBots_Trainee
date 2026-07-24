import java.util.Scanner;
import java.util.concurrent.CyclicBarrier;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class ContarPrimos {
	private static final int Num_threads = 8; //define o número de threads
	public static List<Integer> resultados = Collections.synchronizedList(new ArrayList<>());//lista sincronizada para armazenar os primos
	private static long tempoInicio;
	
	public static void main(String[] args) {	
		int n = 500000;//final do intervalo
		
		int[] limites = new int[Num_threads + 1]; //array que armazena os limites onde começa e termina o pedaço de cada thread
		int tamanhoFatia = (n - 2) / Num_threads; //cada thread possui o mesmo tamanho
		limites[0] = 2; //limite inicial
		
		for(int i=1;i < Num_threads; i++) {
			limites[i] = tamanhoFatia + limites[i-1];
		}
		limites[Num_threads] = n+1; //garante que o n seja incluido na contagem
			
		CyclicBarrier barreira = new CyclicBarrier(Num_threads, new PrintarPrimos());
		
		tempoInicio = System.currentTimeMillis(); //inicializa o timer para verificar o tempo
		
		for(int i=0; i < Num_threads; i++) {
			Thread encontrar = new Thread(new ThreadPrimos(limites[i], limites[i+1], barreira, resultados));//cria as threads para encontrar os primos
			encontrar.start();
		}

	}
	
	public static class PrintarPrimos implements Runnable{
		@Override
		public void run() {
			long tempoFim = System.currentTimeMillis();
            long tempoTotal = tempoFim - tempoInicio;//tempo de execução

            Collections.sort(resultados);//ordena a lista
            System.out.println("Primos encontrados: " + resultados);
            System.out.println("-----------------------------------");
            System.out.println("Tempo de execução: " + tempoTotal + " ms");
		}
	}
}
