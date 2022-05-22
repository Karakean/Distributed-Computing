package app;

import lombok.AllArgsConstructor;

import java.util.HashMap;
import java.util.Map;


public class Warehouse extends Thread {
    private int MAX_CAPACITY = 100;
    private Map<Integer, Integer> warehouse;
    private int amount_in_store;

    @Override
    public void run() {
        System.out.println("Warehouse is opened.");
    }

    Warehouse() {
        this.warehouse = new HashMap<>();
        this.amount_in_store = 0;
    }

    synchronized int receiveProducts(int productID, int received_amount) {
        String product="";
        switch (productID) {
            case 0 -> product = "flour";
            case 1 -> product = "olive oil";
            default -> product = "tomatoes";
        }
        if (this.amount_in_store + received_amount <= this.MAX_CAPACITY) {
            this.amount_in_store += received_amount;
            int new_amount = received_amount;
            if (this.warehouse.containsKey(productID)) {
                new_amount += this.warehouse.get(productID);
                this.warehouse.remove(productID);
            }
            this.warehouse.put(productID, new_amount);
            printStore();
            System.out.println(received_amount + " pieces of " + product + " has been added to warehouse.");
            return received_amount;
        } else if (this.amount_in_store != this.MAX_CAPACITY) {
            int amount = this.MAX_CAPACITY - this.amount_in_store;
            this.amount_in_store = this.MAX_CAPACITY;
            int new_amount = amount;
            if (this.warehouse.containsKey(productID)) {
                new_amount += this.warehouse.get(productID);
                this.warehouse.remove(productID);
            }
            this.warehouse.put(productID, new_amount);
            System.out.println("Only " + amount + " out of " + received_amount + " pieces of " + product +
                    " has been added to warehouse due to lack of space.");
            printStore();
            return amount;
        } else {
            System.out.println("Warehouse is full.");
            return 0;
        }
    }

    private void printStore() {
        System.out.println("There is " + this.amount_in_store + " products in the warehouse: ");
        String product = "";
        for (Map.Entry<Integer, Integer> entry : this.warehouse.entrySet()) {
            switch (entry.getKey()) {
                case 0 -> product = "flour";
                case 1 -> product = "olive oil";
                default -> product = "tomatoes";
            }
            System.out.println(product+": "+entry.getValue());
        }
    }

    synchronized int sellProducts(int productID, int amount) {
        String product="";
        switch (productID) {
            case 0 -> product = "flour";
            case 1 -> product = "olive oil";
            default -> product = "tomatoes";
        }
        if (this.warehouse.containsKey(productID)) {
            if (this.warehouse.get(productID) >= amount) {
                int old_amount = this.warehouse.get(productID);
                this.warehouse.remove(productID);
                if(old_amount > amount) {
                    this.warehouse.put(productID, old_amount - amount);
                }
                this.amount_in_store -= amount;
                System.out.println(amount + " pieces of " + product + " has been sold.");
                System.out.println("There is " + this.amount_in_store + " products in the warehouse.");
                return amount;
            } else {
                int old_amount = this.warehouse.get(productID);
                this.warehouse.remove(productID);
                System.out.println(old_amount + " pieces of " + product +" has been sold. There is no " + product + " left.");
                System.out.println("There is " + this.amount_in_store + " products in the warehouse.");
                this.amount_in_store -= old_amount;
                return old_amount;
            }
        } else {
            System.out.println("There is no " + product + " in the warehouse.");
            return 0;
        }
    }
}