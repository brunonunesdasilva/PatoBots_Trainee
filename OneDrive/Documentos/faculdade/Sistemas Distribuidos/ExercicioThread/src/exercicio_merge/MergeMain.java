package exercicio_merge;

public class MergeMain {
	public static void main(String[] args) {
		int[] vector = {1,3,9,6,15,2,8,44,12};
		int ini = 0;
		int end = vector.length - 1;
		int mid = (ini + end) / 2;
		
		MergeThread t1 = new MergeThread(vector, ini, mid);
		MergeThread t2 = new MergeThread(vector, mid + 1, end);
		
		t1.start();
		t2.start();
		
		try {
			t1.join();
			t2.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		Merge.executarMerge(vector, ini, mid, end);
		
		for (int i: vector) System.out.print(i + " ");
	}
}
