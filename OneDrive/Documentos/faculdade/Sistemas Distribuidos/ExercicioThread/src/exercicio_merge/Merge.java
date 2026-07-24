package exercicio_merge;

public class Merge {
	public static void executarMerge(int[] vector, int ini, int mid, int end) {
		int n1 = mid - ini + 1;
		int n2 = end - mid;
		
		int[] L = new int[n1];
		int[] R = new int[n2];
		
		for (int i=0 ; i < n1; i++) L[i] = vector[ini + i];
		for (int j=0; j < n2; j++) R[j] = vector[mid + 1 + j];
		
		int i=0, j=0;
		int k = ini;
		
		while (i < n1 && j < n2) {
			if (L[i] <= R[j]) {
				vector[k] = L[i];
				i++;
			}
			else {
				vector[k] = R[j];
				j++;
			}
			k++;
		}
		
		while (i < n1) {
			vector[k] = L[i];
			i++; k++;
		}
		while (j < n2) {
			vector[k] = R[j];
			j++;k++;
		}
	}
}
