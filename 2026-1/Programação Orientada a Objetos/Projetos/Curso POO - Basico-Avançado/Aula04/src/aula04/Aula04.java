package aula04;

public class Aula04 {
    public static void main(String[] args) {
        Caneta c1 = new Caneta("BIC", "preta", 0.5f, true);
        c1.status();
        Caneta c2 = new Caneta ("KKK", "Laranja", 0.9f, false);
        c2.status();
    }
}