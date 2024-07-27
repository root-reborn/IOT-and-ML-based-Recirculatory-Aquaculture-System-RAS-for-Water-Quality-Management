import React, { useEffect, useRef } from "react";

const LoginComponent = () => {
  const googleButton = useRef(null);

  useEffect(() => {
    const initializeGoogleSignIn = () => {
      window.google.accounts.id.initialize({
        client_id: "YOUR_GOOGLE_CLIENT_ID",
        callback: handleGoogleCredentialResponse,
      });
      window.google.accounts.id.renderButton(
        googleButton.current,
        { theme: "outline", size: "large" } // Customization attributes
      );
    };

    window.google.accounts.id.initialize().then(initializeGoogleSignIn);
  }, []);

  const handleGoogleCredentialResponse = (response) => {
    // Handle successful Google login
    console.log("Google login successful:", response);
    // You can perform additional operations here, such as storing the user's information or redirecting to another page
  };

  return (
    <div>
      <h2>Login with Google</h2>
      <div ref={googleButton}></div>
    </div>
  );
};

export default LoginComponent;
