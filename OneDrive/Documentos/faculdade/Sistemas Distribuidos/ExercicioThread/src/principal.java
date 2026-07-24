
public class principal extends Thread {
	private int start, end;
	private static int sum = 0;
	
	public principal(int start, int end) {
		this.start = start;
		this.end = end;
	}
	
	@Override
	public void run() {
		for (int i = start; i <= end; i++) {
			sum += i;
		}
	}
	
	public static int getSum() {
		return sum;
	}
	
	public static void main(String[] args) {
	    // Criando duas instâncias (threads) para somar intervalos diferentes
	    principal t1 = new principal(1, 50);
	    principal t2 = new principal(51, 100);

	    t1.start(); // Inicia a primeira thread
	    t2.start(); // Inicia a segunda thread

	    try {
	        // Espera as threads terminarem antes de imprimir o resultado
	        t1.join();
	        t2.join();
	    } catch (InterruptedException e) {
	        e.printStackTrace();
	    }

	    System.out.println("A soma total é: " + getSum());
	}
		
}
