import java.util.HashSet;
import java.util.Set;
import java.util.concurrent.*;

/**
 * Created by Norbert on 2016-11-29.
 */
public class Main {

  private static final int NTHREADS_COUNT = 50;
  private static final int NTHREADS_POOL = 10;

  public static void main(String[] args) {

    double xp = 0, xk = 3.14, dx = 0.01, dl = (xk - xp) / NTHREADS_COUNT;

    ExecutorService pool = Executors.newFixedThreadPool(NTHREADS_POOL);
    Set<Future<Double>> set = new HashSet<>();
    for (int i = 0; i < NTHREADS_COUNT; i++) {
      double a = xp + i * dl;
      double b = xp + (i + 1) * dl;
      Callable<Double> callable = new Calka_callable(a, b, dx);
      Future<Double> future = pool.submit(callable);
      set.add(future);
    }

    pool.shutdown();

    // Wait until all threads finish
    while (!pool.isTerminated()) {
    }

    double sum = 0;
    for (Future<Double> future : set) {
      try {
        sum += future.get();
      } catch (InterruptedException | ExecutionException a) {
      }
    }
    System.out.printf("Suma całek cząstkowych to %s%n", sum);

    Calka_callable calka = new Calka_callable(xp, xk, dx);
    System.out.printf("Całka sekwencyjna to %s%n", calka.compute());

    System.exit(0);
  }
}
