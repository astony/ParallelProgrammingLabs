public class Watek2D extends Thread {

  Obraz obraz;
  char[] znak;
  int row_p;
  int row_k;
  int col_p;
  int col_k;
  public int[] count_znak_2d;

  Watek2D(Obraz obraz, char[] znak, int row_p, int row_k, int col_p,
          int col_k) {
    this.obraz = obraz;
    this.znak = znak;
    this.row_p = row_p;
    this.row_k = row_k;
    this.col_p = col_p;
    this.col_k = col_k;
    count_znak_2d = new int[znak.length];
  }

  public void run() {
    for (int i = 0; i < znak.length; ++i)
      count_znak_2d[i] =
          obraz.calculate_sing_row_column(znak[i], row_p, row_k, col_p, col_k);
  }
}
