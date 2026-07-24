package sequencial;

public class primosSequencial {

    public static void main(String[] args) {
    	
    	int limite = 500000;//final do intervalo
        
        long tempoInicio = System.currentTimeMillis();

        if(limite >= 2) {
        	System.out.println("2 ");// imprime o número 2, que é o único número primo par
        }

        for (int i = 3; i <= limite; i+=2) {// percorre os números ímpares a partir de 3 até o limite, pulando os pares, verificando se cada número é primo
            if (verificaPrimos(i)) {
                System.out.println(i);
            }
        }
        long tempoFim = System.currentTimeMillis();
        long tempoTotal = tempoFim - tempoInicio;
        System.out.println("Tempo de execução: " + tempoTotal + " ms");
    }

    public static boolean verificaPrimos(int numero) {
        if (numero <= 1)
            return false;
        
        for (int i = 3; i <= Math.sqrt(numero); i += 2) {// verifica se o numero é divisível por algum número ímpar até a raiz quadrada do número
            if (numero % i == 0) {
                return false;
            }
        }
        return true;
    }
}