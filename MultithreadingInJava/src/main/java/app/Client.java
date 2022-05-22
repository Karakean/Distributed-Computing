package app;

import lombok.AllArgsConstructor;

@AllArgsConstructor
public class Client extends Thread {
    private Warehouse warehouse;
    private byte ID;

    @Override
    public void run(){
        while (true) {
            try {
                sleep((long)(Math.random() * 5000));
                String product = "";
                int result = (int)(Math.random() * 3);
                switch (result) {
                    case 0 -> product = "flour";
                    case 1 -> product = "olive oil";
                    default -> product = "tomatoes";
                }
                int amount = 1 + (int)(Math.random()*10);
                System.out.println("Client" + ID + " attempts to buy "+amount+" pieces of " + product);
                int available_amount = warehouse.sellProducts(result, amount);
                if (amount == available_amount) {
                    System.out.println("Client" + ID + " bought "+amount+" pieces of " + product + " as he wished");
                }
                else if (available_amount > 0){
                    System.out.println("Client" + ID +" could buy only " + available_amount + " of " + product + " due to " +
                            "warehouse's stock shortages");
                }
                else {
                    System.out.println("Client" + ID +" could not buy anything. There is no " + product + " in the warehouse");
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}