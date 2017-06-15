import bb.cascades 1.4

Container {
    id: root
    
    property string value: ""
    property string result: ""
    
    function resetText() {
        root.value = "";
        textField.resetText();
    }
    
    function requestFocus() {
        textField.requestFocus();
    }
    
    function isValid() {
        return textField.validator.state === ValidationState.Valid;
    }
    
    function validate() {
        textField.validator.validate();
    }
    
    Container {
        leftPadding: ui.du(2)
        topPadding: ui.du(2)
        Label {
            text: qsTr("Name") + Retranslate.onLocaleOrLanguageChanged
        }
    }
    
    TextField {
        id: textField
        text: value
        inputMode: TextFieldInputMode.Text
        
        validator: Validator {
            errorMessage: qsTr("This field cannot be empty") + Retranslate.onLocaleOrLanguageChanged
            mode: ValidationMode.Custom
            onValidate: {
                if (root.result.trim() === "") {
                    state = ValidationState.Invalid;
                } else {
                    state = ValidationState.Valid;
                }
            }
        }
        
        onTextChanging: {
            root.result = text;
        }
    }
    
    onCreationCompleted: {
        root.result = root.value;
    }
    
    onValueChanged: {
        textField.text = value;
    }
}
