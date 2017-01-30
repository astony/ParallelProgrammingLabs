import java.util.concurrent.Callable;

/**
 * Created by Norbert on 2016-11-29.
 */
public class Calka_callable implements Callable<Double> {

  private double dx;
  private double xp;
  private double xk;
  private int N;

  public Calka_callable(double xp, double xk, double dx) {
    this.xp = xp;
    this.xk = xk;
    this.N = (int)Math.ceil((xk - xp) / dx);
    this.dx = (xk - xp) / N;
    System.out.println("Creating an instance of Calka_callable");
    System.out.println("xp = " + xp + ", xk = " + xk + ", N = " + N);
    System.out.println("dx requested = " + dx + ", dx final = " + this.dx);
  }

  private double getFunction(double x) { return Math.sin(x); }

  public double compute() {
    double calka = 0;
    int i;
    for (i = 0; i < N; i++) {
      double x1 = xp + i * dx;
      double x2 = x1 + dx;
      calka += ((getFunction(x1) + getFunction(x2)) / 2.) * dx;
    }
    return calka;
  }

  @Override
  public Double call() throws Exception {
    double calka = compute();
    System.out.println("Calka czastkowa: " + calka);
    return calka;
  }
}
