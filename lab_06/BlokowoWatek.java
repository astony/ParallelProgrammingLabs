class BlokowoWatek implements Runnable {

  Obraz obraz;
  char[] znak;

  BlokowoWatek(Obraz obraz, char[] znak) {
    this.obraz = obraz;
    this.znak = znak;
  }

  public void run() {
    int suma;
    for (int k = 0; k < znak.length; k++) {
      suma = obraz.calculate_sign(znak[k]);
      synchronized (System.out) {
        System.out.print("Watek " + znak[k] + " : " + suma + " ");
        for (int i = 0; i < suma; ++i)
          System.out.print("=");
        System.out.println("");
      }
    }
  }
}