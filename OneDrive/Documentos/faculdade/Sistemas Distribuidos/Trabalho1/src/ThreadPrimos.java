import java.util.concurrent.CyclicBarrier;
import java.util.List;

public class ThreadPrimos implements Runnable {
	private int inicio, fim;
	private List<Integer> lista;
	private CyclicBarrier barreira;
	
	public ThreadPrimos (int inicio, int fim, CyclicBarrier barreira, List<Integer> lista) {
		this.inicio = inicio;
		this.fim = fim;
		this.barreira = barreira;
		this.lista = lista;
	}
	@Override
	public void run() {
		try {
			for (int i = inicio; i < fim; i++) {//percorre o intervalo da thread
	            // Pula os pares (exceto o 2)
	            if (i > 2 && i % 2 == 0) continue; 
	            
	            // Se for o 2 ou for ímpar, faz a verificação
	            if (verificar_primo(i)) {
	                lista.add(i);//adiciona a lista sincronizada
	            }
	        }
			barreira.await();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private boolean verificar_primo(int n) {
	    if (n < 2) return false;
	    if (n == 2) return true;

	    int limite = (int) Math.sqrt(n);
	    for (int j = 3; j <= limite; j += 2) {//verifica se o numero é divisível por algum número ímpar até a raiz quadrada do número
	        if (n % j == 0) return false;
	    }
	    return true;
	}
}
