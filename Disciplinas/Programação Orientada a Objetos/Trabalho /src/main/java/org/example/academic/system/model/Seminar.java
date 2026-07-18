package org.example.academic.system.model;

import lombok.ToString;

@ToString(callSuper = true)
public class Seminar extends Assessment {
    public Seminar(Double value, Double weight) { super(value, weight); }
    @Override public String getType() { return "Seminar"; }
}
