package exercicio_merge;

public class MergeThread extends Thread {
	private int[] vector;
	private int ini, end;
	
	public MergeThread(int[] vector, int ini, int end) {
		this.vector = vector;
		this.ini = ini;
		this.end = end;
	}
	
	@Override
	public void run() {
		sort(vector, ini, end);
	}
	
	private void sort(int[] v, int p, int r) {
		if (p < r) {
			int q = (p + r) / 2;
			sort(v, p, q);
			sort(v, q + 1, r);
			Merge.executarMerge(v,p,q,r);
		}
	}
}
