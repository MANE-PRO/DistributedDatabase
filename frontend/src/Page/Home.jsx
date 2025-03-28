import React from "react";
import styles from "./Home.module.css";
import Button from "../components/Button";
import Modal from "./Modal";
import { useState } from "react";
import RecordsTable from "./RecordsTable";
const Home = ({ records, onDelete, handleModal }) => {
    
    return (
        <>
            <div className={styles["no-table-container"]}>
            {records.length === 0 ? <p className={styles["no-table-text"]}>No Record Found</p> : <RecordsTable records={records} onDelete={onDelete}/>}
            <Button onClick={handleModal}>+ Create New Record</Button>
            </div>
        </>
    );
}

export default Home;