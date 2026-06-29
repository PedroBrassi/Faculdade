package org.example.academic.system.model;

import lombok.ToString;

@ToString(callSuper = true)
public class Assignment extends Assessment {
    public Assignment(Double value, Double weight) { super(value, weight); }
    @Override public String getType() { return "Assignment"; }
}
