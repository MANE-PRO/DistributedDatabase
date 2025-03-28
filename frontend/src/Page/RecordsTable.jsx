import React from "react";
import styles from "./RecordsTable.module.css"; // Import the CSS file

const RecordsTable = ({ records, onDelete }) => {
  return (
      <table className={styles["records-table"]}>
        <thead>
          <tr>
            <th>ID Number</th>
            <th>Name</th>
            <th></th>
          </tr>
        </thead>
        <tbody>
          {records.map((record) => (
            <tr key={record.id} className={styles["table-row"]}>
              <td>{record.id}</td>
              <td>{record.name}</td>
              <td>
                <button className={styles["delete-button"]} onClick={() => onDelete({id: record.id, name:record.name})}>
                  Delete
                </button>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
  );
};

export default RecordsTable;
