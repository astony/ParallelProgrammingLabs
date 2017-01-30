class JedenZnakWatek extends Thread {

  Obraz obraz;
  char znak;

  JedenZnakWatek(Obraz obraz, char znak) {
    this.obraz = obraz;
    this.znak = znak;
  }

  public void run() {
    int suma = obraz.calculate_sign(znak);

    synchronized (System.out) {
      System.out.print("Watek " + znak + " : " + suma + " ");
      for (int i = 0; i < suma; ++i)
        System.out.print("=");
      System.out.println("");
    }
  }
}