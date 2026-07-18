package org.example.academic.system.model;

import lombok.ToString;

@ToString(callSuper = true)
public class PracticalAssignment extends Assessment {
    public PracticalAssignment(Double value, Double weight) { super(value, weight); }
    @Override public String getType() { return "PracticalAssignment"; }
}
