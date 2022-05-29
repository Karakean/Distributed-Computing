package app;

import lombok.AllArgsConstructor;

@AllArgsConstructor
public class Client extends Thread {
    private Warehouse warehouse;
    private byte ID;
    private final Object lock;

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
                //System.out.println("Client" + ID + " attempts to buy "+amount+" pieces of " + product);
                int availableAmount = warehouse.sellProducts(product, amount);
                if (amount == availableAmount) {
                    //System.out.println("Client" + ID + " bought "+amount+" pieces of " + product + " as he wished");
                }
                else if (availableAmount > 0){
                    //System.out.println("Client" + ID +" could buy only " + availableAmount + " of " + product + " due to " +
                    //        "warehouse's stock shortages");
                }
                else {
                    System.out.println("Client" + ID +" could not buy what he wanted to. There is no " + product +
                            " in the warehouse. He is waiting...");
                    synchronized (lock) {
                        while ((availableAmount=warehouse.sellProducts(product, amount))==0) {
                            lock.wait();
                        }
                        System.out.println("Client"+ID+" after waiting bought "+availableAmount+" pieces of "+product);
                    }
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}