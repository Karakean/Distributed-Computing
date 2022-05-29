package app;

import java.util.ArrayList;

public class Application{
    private static final ArrayList<Producer> producers = new ArrayList<>();
    private static final ArrayList<Client> clients = new ArrayList<>();
    private static final Object lock = new Object();

    public static void main(String[] args) {

        Warehouse warehouse = new Warehouse(lock);
        warehouse.start();

        for(byte i=0; i<3; i++) {
            producers.add(new Producer(warehouse, i, lock));
            producers.get(i).start();
            clients.add(new Client(warehouse, i, lock));
            clients.get(i).start();
        }
    }
}