import java.util.Scanner;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

class Histogram {

  private static final int NTHREADS_POOL = 10;

  public static void main(String[] args) {
    Scanner scanner = new Scanner(System.in);

    System.out.println("Set image size: n (#rows), m(#kolumns)");
    int n = scanner.nextInt();
    int m = scanner.nextInt();
    Obraz obraz_1 = new Obraz(n, m);

    obraz_1.calculate_histogram();
    obraz_1.print_histogram();

    char[] znak = new char[94];

    System.out.println("\n\nJedenZnak:\n\n");

    ExecutorService pool1 = Executors.newFixedThreadPool(NTHREADS_POOL);

    for (int i = 0; i < 94; i++) {
      znak[i] = (char)(i + 33); // ascii 33-127
      Runnable runnable = new JedenZnakWatek(obraz_1, znak[i]);
      pool1.execute(runnable);
    }

    pool1.shutdown();

    while (!pool1.isTerminated()) {
    }

    //		System.out.println("\n\nBlokowo:\n\n");
    //
    //		int num_threads = 4;
    //		Thread[] BlokowoThr = new Thread[num_threads];
    //		int rozmiar = znak.length / num_threads;
    //		int a , b;
    //
    //		for(int i = 0; i < num_threads; ++i)
    //		{
    //			a = i == 0 ? 0 : (i)*rozmiar + 1;
    //			b = i == num_threads-1 ? znak.length : (i+1)*rozmiar;
    //			//System.out.println("a = " + a + " b = " + b);
    //			BlokowoWatek runnable = new BlokowoWatek(obraz_1,
    // Arrays.copyOfRange(znak, a, b));
    //			BlokowoThr[i] = new Thread(runnable);
    //			BlokowoThr[i].start();
    //		}
    //
    //		for (int i = 0; i < num_threads; i++)
    //		{
    //		    try
    //			{
    //				BlokowoThr[i].join();
    //		    }
    //			catch (InterruptedException e)
    //			{
    //			}
    //		}

    System.out.println("\n\nWierszowo:\n\n");

    ExecutorService pool2 = Executors.newFixedThreadPool(NTHREADS_POOL);

    Set<Future<int[]>> set = new HashSet<>();

    for (int i = 0; i < obraz_1.size_n; ++i) {
      Callable<int[]> callable = new WierszowyWatek(obraz_1, znak, i);
      Future<int[]> future = pool2.submit(callable);
      set.add(future);
    }

    pool2.shutdown();

    while (!pool2.isTerminated()) {
    }

    int[] tmp;

    int[] suma = new int[znak.length];
    for (int i = 0; i < znak.length; ++i)
      suma[i] = 0;

    for (Future<int[]> future : set) {
      try {
        tmp = future.get();
        for (int j = 0; j < tmp.length; ++j) {
          suma[j] += tmp[j];
        }
      } catch (InterruptedException | ExecutionException aa) {
      }
    }

    for (int i = 0; i < znak.length; ++i) {
      System.out.print("Wątek " + znak[i] + " : " + suma[i] + " ");
      for (int j = 0; j < suma[i]; ++j)
        System.out.print("=");
      System.out.println("");
    }
    //
    //		System.out.println("\n\n2D:\n\n");
    //
    //		int threads = 4;
    //
    //		Watek2D[] Thr2D = new Watek2D[threads];
    //
    //		int row_count = obraz_1.size_n / threads;
    //		int column_count = obraz_1.size_m / threads;
    //
    //		for(int i = 0; i < threads; ++i)
    //			for(int j = 0; j < threads; ++j)
    //			{
    //				int row_p = i == 0 ? 0 : i * row_count + 1;
    //				int row_k = i == threads-1 ? obraz_1.size_n-1 :
    //(i+1)*row_count;
    //				int col_p = j == 0 ? 0 : j * column_count + 1;
    //				int col_k = j == threads-1 ? obraz_1.size_m-1 :
    //(j+1)*column_count;
    //				(Thr2D[i] = new Watek2D(obraz_1, znak, row_p, row_k,
    //col_p,
    // col_k)).start();
    //			}
    //
    //        for(int i = 0; i < threads; i++)
    //		{
    //		    try
    //			{
    //				Thr2D[i].join();
    //		    }
    //			catch (InterruptedException e)
    //			{
    //			}
    //		}
    //
    //
    //		for(int i = 0; i < threads; ++i)
    //			for(int j = 0; j < znak.length; ++j)
    //				System.out.println("Watek nr " + i + " znak " + znak[j] + "
    //wynik
    //" + Thr2D[i].count_znak_2d[j]);
  }
}
