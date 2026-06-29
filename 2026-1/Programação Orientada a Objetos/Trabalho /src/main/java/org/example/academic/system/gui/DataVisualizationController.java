package org.example.academic.system.gui;

import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.fxml.FXML;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import org.example.academic.system.model.AcademicClass;
import org.example.academic.system.model.Assessment;

/**
 * Tela de visualização de turmas e avaliações (TUS-2413).
 *
 * <p>Lê os dados do backend ({@code AcademicSystemController#listClasses}) e os
 * exibe em duas tabelas: uma de turmas e outra com todas as avaliações
 * cadastradas.</p>
 */
public class DataVisualizationController {

    /** Linha da tabela de avaliações (achatada para exibição). */
    public record AssessmentRow(String classCode, String type, String value, String weight) {
    }

    @FXML private TableView<AcademicClass> tableClasses;
    @FXML private TableView<AssessmentRow> tableAssessments;

    @FXML
    @SuppressWarnings("unchecked")
    public void initialize() {
        TableColumn<AcademicClass, String> colCode = new TableColumn<>("Código");
        colCode.setCellValueFactory(c -> new SimpleStringProperty(c.getValue().getCode()));
        TableColumn<AcademicClass, String> colTitle = new TableColumn<>("Título");
        colTitle.setCellValueFactory(c -> new SimpleStringProperty(c.getValue().getTitle()));
        TableColumn<AcademicClass, String> colCount = new TableColumn<>("Avaliações");
        colCount.setCellValueFactory(c ->
                new SimpleStringProperty(String.valueOf(c.getValue().getAssessments().size())));
        tableClasses.getColumns().setAll(colCode, colTitle, colCount);

        TableColumn<AssessmentRow, String> aCode = new TableColumn<>("Turma");
        aCode.setCellValueFactory(c -> new SimpleStringProperty(c.getValue().classCode()));
        TableColumn<AssessmentRow, String> aType = new TableColumn<>("Tipo");
        aType.setCellValueFactory(c -> new SimpleStringProperty(c.getValue().type()));
        TableColumn<AssessmentRow, String> aValue = new TableColumn<>("Nota");
        aValue.setCellValueFactory(c -> new SimpleStringProperty(c.getValue().value()));
        TableColumn<AssessmentRow, String> aWeight = new TableColumn<>("Peso");
        aWeight.setCellValueFactory(c -> new SimpleStringProperty(c.getValue().weight()));
        tableAssessments.getColumns().setAll(aCode, aType, aValue, aWeight);

        refreshData();
    }

    @FXML
    private void refreshData() {
        var classes = AppContext.academic().listClasses();
        tableClasses.setItems(FXCollections.observableArrayList(classes));

        var rows = FXCollections.<AssessmentRow>observableArrayList();
        for (AcademicClass cls : classes) {
            for (Assessment a : cls.getAssessments()) {
                rows.add(new AssessmentRow(
                        cls.getCode(),
                        a.getType(),
                        String.valueOf(a.getValue()),
                        String.valueOf(a.getWeight())));
            }
        }
        tableAssessments.setItems(rows);
    }

    @FXML
    private void goBack() {
        SceneManager.switchScene("MainScreen.fxml", "Sistema Acadêmico");
    }
}
