import java.util.concurrent.Callable;

public class WierszowyWatek implements Callable<int[]> {

  Obraz obraz;
  char[] znak;
  int row;
  int[] count_znak_row;

  WierszowyWatek(Obraz obraz, char[] znak, int row) {
    this.obraz = obraz;
    this.znak = znak;
    this.row = row;
    this.count_znak_row = new int[znak.length];
  }

  @Override
  public int[] call() {
    for (int i = 0; i < znak.length; ++i)
      count_znak_row[i] = obraz.calculate_sing_row(znak[i], row);
    return count_znak_row;
  }
}
