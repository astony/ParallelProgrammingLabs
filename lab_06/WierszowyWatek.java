public class WierszowyWatek extends Thread {

  Obraz obraz;
  char[] znak;
  int row;
  public int[] count_znak_row;

  WierszowyWatek(Obraz obraz, char[] znak, int row) {
    this.obraz = obraz;
    this.znak = znak;
    this.row = row;
    this.count_znak_row = new int[znak.length];
  }

  public void run() {
    for (int i = 0; i < znak.length; ++i)
      count_znak_row[i] = obraz.calculate_sing_row(znak[i], row);
  }
}
