import java.util.Random;

class Obraz {

  public int size_n;
  public int size_m;
  private char[][] tab;
  private int[] histogram;

  public Obraz(int n, int m) {
    this.size_n = n;
    this.size_m = m;
    tab = new char[n][m];

    final Random random = new Random();

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        tab[i][j] = (char)(random.nextInt(94) + 33); // ascii 33-127
        System.out.print(tab[i][j] + " ");
      }
      System.out.print("\n");
    }

    System.out.print("\n\n");

    histogram = new int[94];
    clear_histogram();
  }

  public void clear_histogram() {
    for (int i = 0; i < 94; i++)
      histogram[i] = 0;
  }

  public void calculate_histogram() {
    for (int i = 0; i < size_n; i++)
      for (int j = 0; j < size_m; j++)
        for (int k = 0; k < 94; k++)
          if (tab[i][j] == (char)(k + 33))
            histogram[k]++;
  }

  public int calculate_sign(char znak) {
    int suma = 0;
    for (int i = 0; i < size_n; ++i)
      for (int j = 0; j < size_m; ++j)
        if (tab[i][j] == znak)
          ++suma;
    return suma;
  }

  public int calculate_sing_row(char znak, int row) {
    if (row > size_n)
      return 0;

    int suma = 0;
    for (int i = 0; i < size_n; ++i)
      if (tab[row][i] == znak)
        ++suma;
    return suma;
  }

  public int calculate_sing_row_column(char znak, int row_p, int row_k,
                                       int col_p, int col_k) {
    int suma = 0;
    for (int i = row_p; i <= row_k; ++i)
      for (int j = col_p; j < col_k; ++j)
        if (tab[i][j] == znak)
          ++suma;
    return suma;
  }

  public void print_histogram() {
    System.out.print("\n");
    for (int i = 0; i < 94; i++) {
      System.out.print((char)(i + 33) + " " + histogram[i] + "\n");
    }
  }
}
