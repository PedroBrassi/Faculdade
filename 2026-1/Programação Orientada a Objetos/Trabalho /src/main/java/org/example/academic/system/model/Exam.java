package org.example.academic.system.model;

import lombok.ToString;

@ToString(callSuper = true)
public class Exam extends Assessment {
    public Exam(Double value, Double weight) { super(value, weight); }
    @Override public String getType() { return "Exam"; }
}
