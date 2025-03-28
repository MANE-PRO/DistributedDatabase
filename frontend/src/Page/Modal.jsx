import React, { useState, useRef } from "react";
import styles from "./Modal.module.css"; // Import CSS module
import ReactDOM from "react-dom";
import Button from "../components/Button";
const Modal = ({ isOpen, onClose, onSave }) => {

  const [error, setError] = useState("");
  const id = useRef("");
  const name = useRef("name");

  if (!isOpen) return null;
  const saveHandler = () =>{
    if(id.current.value === "" || name.current.value === ""){
      setError("Enter the fields correctly.");
      return;
    }
    onSave({id: id.current.value, name: name.current.value});
  }

  return (
    <div className={styles.modalOverlay}>
      <div className={styles.modal}>
        {/* Cross Button to Close Modal */}
        <button className={styles.closeButton} onClick={onClose}>✖</button>

        <h2 className={styles.modalTitle}>Create New Record</h2>
        {error && (
          <div className={styles.errorMessage}>
            <span>{error}</span>
            <button className={styles.errorClose} onClick={() => setError("")}>✖</button>
          </div>
        )}

        <div className={styles.formGroup}>
          <input
            type="text"
            className={styles.input}
            placeholder="Enter ID Number"
            ref={id}
          />
        </div>

        <div className={styles.formGroup}>
          <input
            type="text"
            className={styles.input}
            placeholder="Enter Name"
            ref = {name}
          />
        </div>
        {/* Buttons */}
        <div className={styles.buttonContainer}>
          <button onClick={saveHandler}>Save</button>
        </div>
      </div>
    </div>
  );
};

export default Modal;