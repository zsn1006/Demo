package com.polycom.polycom.bean;

/**
 * Created by zsn on 2016/7/11.
 */
public class SqlBean {

    private String author;
    private double price;
    private String name;
    private int pages;

    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getPages() {
        return pages;
    }

    public void setPages(int pages) {
        this.pages = pages;
    }

    @Override
    public String toString() {
        return "SqlBean{" +
                "author='" + author + '\'' +
                ", price=" + price +
                ", name='" + name + '\'' +
                ", pages=" + pages +
                '}';
    }
}
