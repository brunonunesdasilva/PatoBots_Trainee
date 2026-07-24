package InverterMatriz;

public class EscreverMatriz implements Runnable{
	private int[][] matriz;
	
	EscreverMatriz(int[][] matriz) {
		this.matriz = matriz;
	}
	
	@Override
	public void run() {
		for(int i=0; i<matriz.length; i++) {
			for(int j=0; j< matriz[i].length; j++){
				System.out.print(matriz[i][j] + " ");
				if(j == matriz[i].length - 1) {
					System.out.print("\n");
				}
			}
		}
	}
}
